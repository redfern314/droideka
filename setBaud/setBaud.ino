#include <SoftwareSerial.h>

SoftwareSerial blueSerial(12,13);

void setup(){
  blueSerial.begin(9600);
  Serial.begin(9600);
  blueSerial.write("AT+BAUD4\r\n");
  blueSerial.write("AT+UART=9600,0,0\r\n");
}

void loop(){
  while (blueSerial.available()){
    Serial.write(blueSerial.read());
  }
  while (Serial.available()){
    blueSerial.write(Serial.read());
  }
}