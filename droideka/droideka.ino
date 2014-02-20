/*
 * HackMIT Droideka
 * 2/15/14
 * Forrest Bourke, Derek Redfern, Eric Schneider, Paul Titchener
 */


byte controlData[3];
unsigned long start;


// run once
void setup() {     
  Serial.begin (115200);
  Serial.println("Start");

  setupActuators();
  setupWifi();
  
  start = millis();

}

void loop() {
  getControlData(controlData);
  prettyPrintControl(controlData);
  setMotorMode(controlData[2]);
  setMotorSpeed(controlData[0]);
  setTiltServoState(controlData[1]);
  
  Serial.print(F("Time since start:"));
  Serial.println(millis()-start);
  delay(100);
}
