/*
 * HackMIT Droideka
 * 2/15/14
 * Forrest Bourke, Derek Redfern, Eric Schneider, Paul Titchener
 */

#define wifiEnable          4
#define directionalSwitch   6
#define accelCS             A3
#define motorAm1            7
#define motorAm2            8
#define motorPWM            9
#define wifiCS              10
#define rotaryPin1          A0
#define rotaryPin2          A1
#define rotaryPort          PINC

#define FORWARD             0
#define BACKWARD            1
#define STOP                2

int motorDirections = FORWARD;

// run once
void setup() {     
  pinMode(wifiEnable, OUTPUT);         
  pinMode(accelCS, OUTPUT);
  pinMode(wifiCS, OUTPUT);
  pinMode(motorAm1, OUTPUT);
  pinMode(motorAm2, OUTPUT);
  pinMode(motorPWM, OUTPUT);
  pinMode(wifiCS, OUTPUT);
  pinMode(directionalSwitch, INPUT);

  Serial.begin (115200);
  Serial.println("Start");
}

/* returns change in encoder state (-1,0,1) */
int8_t read_encoder()
{
  static int8_t enc_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
  static uint8_t old_AB = 0;
  /**/
  old_AB <<= 2;                   //remember previous state
  old_AB |= ( rotaryPort & 0x03 );  //add current state
  return ( enc_states[( old_AB & 0x0f )]);
}

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

// run ad nauseum
void loop() {
    int switchState = digitalRead(directionalSwitch);
    if (switchState != motorDirections) {
        motorDirections = switchState;
        setMotorMode(motorDirections);
    }
}
