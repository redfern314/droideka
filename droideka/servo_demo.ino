#include <Servo.h>

//Pin Variables
int servoPin = 11;  //Blue signal

int stopVal = 1500;
int rightVal = 1200;
int leftVal = 2*stopVal-rightVal;

Servo servo;

int currentState = 2; //0-left, 1-right, 2-center
int requestedState = 2;

int stopTime = 99999999; //ghetto as shit


void setup(){  
  servo.attach(servoPin);
  servo.writeMicroseconds(stopVal);
}




void loop(){
  requestedState = 1;
  servo.writeMicroseconds(rightVal);
  // switch(requestedState){
  //   case 0:
  //     currentState = left(currentState);
  //     break;
  //   case 1:
  //     currentState = right(currentState);
  //     break;
  //   case 2:
  //     currentState = center(currentState);
  //     break;
  // }

  // if(millis() > stopTime)
  // {
  //   servo.writeMicroseconds(stopVal);
  // }
}


int left(int state){
  switch(state){
    case 0: //we're already here
      return 0; 
    case 1: //we're going from the right to the left
      servo.writeMicroseconds(leftVal);
      stopTime = millis() + 1000; //run to the left for one second
      return 1;
    case 2: //we're going from the right to the center
      servo.writeMicroseconds(leftVal);
      stopTime = millis() + 500; //run to the left for half a second
      return 2;
  }
}

int right(int state){
  switch(state){
    case 0: //we're going from left to right
      servo.writeMicroseconds(rightVal);
      stopTime = millis() + 1000; //run to the right for one second
      return 0; 
    case 1: //we're already here
      return 1;
    case 2: //we're going from the right to the center
      servo.writeMicroseconds(rightVal);
      stopTime = millis() + 500; //run to the right for half a second
      return 2;
  }
}


int center(int state){
  switch(state){
    case 0: //we're going from center to left
      servo.writeMicroseconds(leftVal);
      stopTime = millis() + 750; //run to the right for .75 second
      return 0; 
    case 1: //we're going from center to right
      servo.writeMicroseconds(rightVal);
      stopTime = millis() + 750; //run to the right for .75 second
      return 1;
    case 2: //we're going from the center to the center
      return 2;
  }
}

  
