/*
 * HackMIT Droideka
 * 2/15/14
 * Forrest Bourke, Derek Redfern, Eric Schneider, Paul Titchener
 */


byte controlData[5];
unsigned long last;

boolean hardcode; // whether to use the hardcoded values


// run once
void setup() {
   
  Serial.begin(9600);
  Serial.println("Start");

  hardcode = false;
  

  setupActuators();

  if (!hardcode){
    setupComs();
  } else {
    byte pen_speed = 0; // 0-255
    byte pen_dir = 0; // 0 or 1
    byte tilt = 0; // 0-255
    byte laser = 0; // 0 is off, non-zero is on.
    byte standing = 0;
    controlData[0] = pen_speed;
    controlData[1] = pen_dir;
    controlData[2] = tilt;
    controlData[3] = laser;
    controlData[4] = standing;
    setMotorSpeed(controlData[0]);
    setTiltServoState(controlData[1]);
    setMotorMode(controlData[2]);
    setLaserState(controlData[3]);
    setStandingState(controlData[4]);
  }
  //last = millis();

}

void loop() {
  if (!hardcode){
    if (getControlData(controlData)){
      prettyPrintControl(controlData);
      setMotorSpeed(controlData[0]);
      setTiltServoState(controlData[1]);
      setMotorMode(controlData[2]);
      setLaserState(controlData[3]);
      setStandingState(controlData[4]);
    }
  }
  
  //Serial.print(F("Cycle Time: "));
  //Serial.println(millis()-last);
  //last = millis();
  delay(1);
}
