#include <SoftwareSerial.h>  
  
SoftwareSerial mySerial(10, 11); // RX, TX  
SoftwareSerial hc12(6, 5); // RX, TX  

String command = ""; 
   
void setup() {
  Serial.begin(115200);  
  Serial.println("Type AT commands!");  
  
  // SoftwareSerial "com port" data rate. JY-MCU v1.03 defaults to 9600.  
  mySerial.begin(9600);  


pinMode(7,OUTPUT);
digitalWrite(7,LOW); // enter AT command mode
Serial.begin(9600);
hc12.begin(9600);
hc12.print(F("AT+C001")); // set to channel 1 
delay(100);
digitalWrite(7,HIGH);// enter transparent mode
}

void loop() {
  <  // Read device output if available.  
  if (mySerial.available()) 
  {  
     while(mySerial.available()) 
     { // While there is more to be read, keep reading.  
       command += (char)mySerial.read();  
     }  
   Serial.println(command);  
   command = ""; // No repeats  
  }  
  
  // Read user input if available.  
  if (Serial.available())
  {  
    delay(10); // The DELAY!  
    mySerial.write(Serial.read());  
  }

    if(Serial.available()) hc12.write(Serial.read());
if(hc12.available()) Serial.write(hc12.read());
}

