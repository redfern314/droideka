/*
 * HackMIT Droideka
 * 2/15/14
 * Forrest Bourke, Derek Redfern, Eric Schneider, Paul Titchener
 */

// pin definitions
#define wifiEnable          4
#define directionalSwitch   6
#define accelCS             A3
#define motorAm1            7
#define motorAm2            8
#define motorPWM            9
#define wifiCS              10
#define rotaryPin1          _BV(PC0) //A0
#define rotaryPin2          _BV(PC1) //A1
#define rotaryPort          PINC

// directional constants
#define FORWARD             0
#define BACKWARD            1
#define STOP                2

// other magic numbers
#define MOTOR_SPEED         255 // 0 to 255

 // WiFi Constants
 #define WLAN_SSID       "MIT"  
 #define WLAN_SECURITY   WLAN_SEC_UNSEC

int motorDirections = FORWARD;

/* returns change in encoder state (-1,0,1) */
int8_t read_encoder()
{
  static int8_t enc_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
  static uint8_t old_AB = 0;
  /**/
  old_AB <<= 2;                   //remember previous state
  old_AB |= ( rotaryPort & (rotaryPin1 | rotaryPin2) );  //add current state
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

void setMotorSpeed(int speed) {
    analogWrite(motorPWM,speed);
}

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

  digitalWrite(wifiEnable,HIGH);

  Serial.begin (115200);
  Serial.println("Start");
}

// run ad nauseum
void loop() {
    int switchState = digitalRead(directionalSwitch);
    if (switchState != motorDirections) {
        motorDirections = switchState;
        setMotorMode(motorDirections);
    }
}
