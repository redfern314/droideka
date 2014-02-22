#include <SoftwareSerial.h>

SoftwareSerial blueSerial(12, 13); // RX, TX

//populates control (a length 3 array) with the control codes
//control[0] = 0-255 (speed)
//control[1] = 0,left; 1,right; 2,center
//control[2] = 0,forward; 1,backward
bool getControlData(byte *control){
  char temp;
  while(blueSerial.available()){
    temp = blueSerial.read();
    delay(1);
    if (temp == 'q' && blueSerial.available() >=3){
      control[0] = blueSerial.read();
      control[1] = blueSerial.read();
      control[2] = blueSerial.read();
      return true;
    } else {
      return false;
    }
  }
}

void setupComs(){
  blueSerial.begin(9600);
}

void prettyPrintControl(byte *control){
  Serial.print(F("Speed Setting: "));
  Serial.println(control[0]);
  Serial.print(F("Turn Setting: "));
  Serial.println(control[1]==0 ? F("Left") : control[1]==1 ? F("Right") : F("Center"));
  Serial.print(F("Direction: "));
  Serial.println(control[2]==0 ? F("Forward") : F("Backward"));
}
