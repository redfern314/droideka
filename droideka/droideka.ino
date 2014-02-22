/*
 * HackMIT Droideka
 * 2/15/14
 * Forrest Bourke, Derek Redfern, Eric Schneider, Paul Titchener
 */


byte controlData[3];
unsigned long start;

boolean hardcode; // whether to use the hardcoded values


// run once
void setup() {
   
  Serial.begin(9600);
  Serial.println("Start");

  hardcode = true;
  
  byte pen_speed = 100; // 0-255
  byte pen_dir = 0; // 0 or 1
  byte tilt = 0; // 0-255

  setupActuators();

  if (!hardcode){
    setupComs();
  } else {
    controlData[0] = pen_speed;
    controlData[1] = pen_dir;
    controlData[2] = tilt;
  }
  
  start = millis();

}

void loop() {
  if (!hardcode){
    getControlData(controlData);
    prettyPrintControl(controlData);
  }
  
  setMotorMode(controlData[2]);
  setMotorSpeed(controlData[0]);
  setTiltServoState(controlData[1]);
  
  Serial.print(F("Time since start:"));
  Serial.println(millis()-start);
  delay(100);
}
