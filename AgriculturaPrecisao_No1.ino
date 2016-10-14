#include <DHT.h>
#include <Wire.h>
#include <RS485.h>

/**
 * Configurações de Pins
 */
const int pinHum1 = A0;
const int pinHum2 = A1;
const int pinHum3 = A2;
const int pinLum = A3;

/* 
 * Configurações da Temperatura 
 */
#define DHTPIN  2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE, 11);


/**
 * Total 
 */
const unsigned char TotalPos = 6;
 
/**
 * Char Result
 */
char result[6];


/**
 * Serial Comunication Address
 */
const unsigned char MASTERADDRESS = 1;
const unsigned char RS485Address = 2;

/**
 * Debug mode
 */
boolean debug= false; 

RS485 line = RS485(10, 11, 3, RS485Address);
   
void setup() {
  Serial.begin(9600);  

  // Init DHT22
  dht.begin();

  // Init Serial Comunication
  line.begin(9600);
  //Wire.begin();
}

void loop() {
  
  readLocalSection();
  delay(500);
  line.waitMessage(onMessageReceived);  
  delay(500);
  
  /*
  Temp(resultTemp);
  Serial.print("Temp: ");
  Serial.println(resultTemp);
  
  Hum(resultHum1, pinHum1);
  Serial.print("Hum1: ");
  Serial.println(resultHum1);
  Hum(resultHum2, pinHum2);
  Serial.print("Hum2: ");
  Serial.println(resultHum2);
  Hum(resultHum3, pinHum3);
  Serial.print("Hum3: ");
  Serial.println(resultHum3);


  int lumT = analogRead(pinLum);
  Serial.print("Lum: ");
  Serial.println(lumT);
  */
  delay(5000);
}

/**
 * onMessageReceived
 * @param      {char}   addressFrom
 * @param      {char*}  buffer
 * @param      {int}    msgSize
 */
void onMessageReceived(unsigned char addressFrom, char * buffer, int msgSize)
{
  char values[1];
  values[0] = buffer[0];
  if (addressFrom != MASTERADDRESS) return;
  if (values[0] == 'Q')
  { 
    
    line.sendMessage(MASTERADDRESS, (char *)result, 200);

    /*
    //Serial.println("values sent:");
    //for (int i = 0, add = I2CADD; i < I2CTotal; i++, add++)
    //{
    //  Serial.print(" { \"add\":");
    //  Serial.print(add, HEX);
    //  Serial.print(", \"A\": ");
    //  Serial.print((int )(byte)sendVals[ i * 2]);
    //  Serial.print(", \"B\": ");
    //  Serial.print((int)(byte)sendVals[ i * 2 + 1]);
    //  Serial.print(" }");
    //  if (i < I2CTotalMinus)
    //    Serial.print(",");
    //}
    //Serial.println();
    */
    return;
  }

  Serial.println("received unkonwn command");
}

void readLocalSection() {

  
  Hum(pinHum1, 0);
  Hum(pinHum2, 1);
  Hum(pinHum3, 2);
  Temp();
  Lum(pinLum);


  if(debug){
    Serial.print("Size: ");
    Serial.print((sizeof(result)/sizeof(int)));
    Serial.print("/");
    Serial.println(sizeof(result));

    
    Serial.print("Hum1: ");
    Serial.println((uint8_t)result[0]);
    Serial.print("Hum2: ");
    Serial.println((uint8_t)result[1]);
    Serial.print("Hum3: ");
    Serial.println((uint8_t)result[2]);
    Serial.print("Temp: ");
    Serial.println((int8_t)result[3]);
    Serial.print("Hum: ");
    Serial.println((int8_t)result[4]);
    Serial.print("Lum: ");
    Serial.println((uint8_t)result[5]);

    
    
    /*
    char* teste = (char *)result;
    Serial.print("Size2: ");
    Serial.println(sizeof(teste));

    Serial.print("XXAXJAXLKJ: ");
    
    Serial.println((int)teste);
    Serial.print("XXAXJAXLKJ1: ");
    Serial.println(teste[0],HEX);
    Serial.print("XXAXJAXLKJ2: ");
    Serial.println(teste[1],HEX);
    Serial.println((unsigned int)teste[1]*256+(unsigned int)teste[0]);
    
    int word1 = teste[1] * 256 + teste[0];
    Serial.print("WORD1: ");
    Serial.println(word1);
    int word2 = ((teste[2] << 16) +(teste[1] << 8) + teste[0]);
    Serial.print("WORD2: ");
    Serial.println(word2);
    */
  }

  /*
  for (byte i = 0, add = I2CADD; i < I2CTotal; i++, add++)
  {
    inputArrs[i * 2] = readI2C(add, GPIOA);
    sendVals[i * 2] = (unsigned char)inputArrs[i * 2];  
    inputArrs[i * 2 + 1] = readI2C(add, GPIOB);
    sendVals[i * 2 + 1] = (unsigned char)inputArrs[i * 2 + 1];
  }*/
  /*Serial.print("values read:");
    
  for (int i = 0, add = I2CADD; i < I2CTotal; i++, add++)
  {
    Serial.print(" { \"add\":");
    Serial.print(add, HEX);
    Serial.print(", \"A\": ");
    Serial.print(inputArrs[i * 2]);
    Serial.print(", \"B\": ");
    Serial.print(inputArrs[i * 2 + 1]);
    Serial.print(" }");
    if (i < I2CTotalMinus)
      Serial.print(",");
  }
  Serial.println();*/
  
}

/**
 * Hum
 * @param      {int}   pin
 * @param      {int}   pos
 */
void Hum(int pin, int pos) { 
  int hum = analogRead(pin);
  result[pos] = (unsigned char)map(hum, 0, 1023, 100, 0);
  //snprintf(result, 200, "%d", hum);
  if(debug){
    Serial.print("#######################Pos: ");
    Serial.println(pos);
    Serial.print("#######################Hum: ");
    Serial.println(map(hum, 0, 1023, 254, 0));
  }
}

/**
 * Temp
 */
void Temp() {  
  float temp = dht.readTemperature();  
  float hum = dht.readHumidity();
  
  result[3] = (int)temp;
  result[4] = (int)hum;

  //snprintf(result, 200, "%d%d", (int)temp, (int)hum);
  if(debug){
    Serial.print("#######################Temp: ");
    Serial.println(temp);
    Serial.print("#######################Hum: ");
    Serial.println(hum);
  }
}

/**
 * Lum
 * @param      {int}   pin
 */
void Lum(int pin) { 
  int lum = analogRead(pin);    
  result[5] = (unsigned char)map(lum, 0, 1023, 100, 0);
  //snprintf(result, 200, "%d", lum);
  if(debug){
    Serial.print("#######################Lum: ");
    Serial.println(lum);
  }
}
