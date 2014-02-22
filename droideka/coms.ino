#include <SoftwareSerial.h>

SoftwareSerial blueSerial(10, 11); // RX, TX

//populates control (a length 3 array) with the control codes
//control[0] = 0-255 (speed)
//control[1] = 0,left; 1,right; 2,center
//control[2] = 0,forward; 1,backward
void getControlData(byte *control){
  if(blueSerial.available() >= 3){
    control[0] = Serial.read();
    control[1] = Serial.read();
    control[2] = Serial.read();
  }
  while(blueSerial.available()){
    blueSerial.read();
  }
}

void setupComs(){
  blueSerial.begin(4800);
}

void prettyPrintControl(byte *control){
  Serial.print(F("Speed Setting: "));
  Serial.println(control[0]);
  Serial.print(F("Turn Setting: "));
  Serial.println(control[1]==0 ? F("Left") : control[1]==1 ? F("Right") : F("Center"));
  Serial.print(F("Direction: "));
  Serial.println(control[2]==0 ? F("Forward") : F("Backward"));
}
