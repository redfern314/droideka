#include "Servo.h"

#define laser               2
#define motorAm1            7
#define motorAm2            8
#define motorPWM            6
#define tiltServoPin        9
#define frontServoPin       10
#define backServoPin        11
#define rotaryPin1          _BV(PC0) //A0
#define rotaryPin2          _BV(PC1) //A1
#define rotaryPort          PINC

// motor directional constants
#define FORWARD             0
#define BACKWARD            1
#define STOP                2

// tilt servo directional constants
#define LEFT                0
#define RIGHT               1
#define CENTER              2

// leg servo state constants
#define UP                  0
#define DOWN                1

#define speedReduction      1

Servo tiltServo;
Servo frontServos;
Servo backServo;
byte state[5];


// note: forward and backward might be switched
void setMotorMode(int mode) {
    if (mode==FORWARD) {
        digitalWrite(motorAm1,HIGH);
        digitalWrite(motorAm2,LOW);
    } else if (mode==BACKWARD) {
        digitalWrite(motorAm1,LOW);
        digitalWrite(motorAm2,HIGH);
    } else if (mode==STOP) {
        digitalWrite(motorAm1,LOW);
        digitalWrite(motorAm2,LOW);
    }
}

void setMotorSpeed(int speed) {
  analogWrite(motorPWM,speed * speedReduction);
}

void setTiltServoState(int tiltState) {
  if (tiltState==LEFT) {
    tiltServo.write(0);
  } else if (tiltState==RIGHT) {
    tiltServo.write(180);
  } else { //center
    tiltServo.write(90);
  }
}


void setupActuators(){
  pinMode(laser, OUTPUT);
  pinMode(motorAm1, OUTPUT);
  pinMode(motorAm2, OUTPUT);
  pinMode(motorPWM, OUTPUT);
  pinMode(frontServoPin, OUTPUT);
  pinMode(backServoPin, OUTPUT);
  tiltServo.attach(tiltServoPin);
  frontServos.attach(frontServoPin);
  backServo.attach(backServoPin);
  setMotorMode(2);
  setMotorSpeed(0);
  setTiltServoState(2);
  setLaserState(0);//laser off
  setStandingState(0);//start rolling
}

void setLaserState(byte on){
  if (on){
    digitalWrite(laser, HIGH);
    state[3] = 1;
  } else {
    digitalWrite(laser, LOW);
    state[3] = 0;
  }
}

void setStandingState(byte standing){
  if (standing){
    frontServos.write(45);
    backServo.write(125);
  } else {
    frontServos.write(125);
    backServo.write(45);
  }  
}