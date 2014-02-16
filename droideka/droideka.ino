/*
 * HackMIT Droideka
 * 2/15/14
 * Forrest Bourke, Derek Redfern, Eric Schneider, Paul Titchener
 */

/* The Servo and Motor definitions */

#include "Servo.h"

// pin definitions
#define wifiEnable          4
// #define accelCS             A3
#define motorAm1            7
#define motorAm2            8
#define motorPWM            6
#define tiltServoPin        9
#define wifiCS              10
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

 // WiFi Constants
 #define WLAN_SSID       "MIT"  
 #define WLAN_SECURITY   WLAN_SEC_UNSEC



/* The Wifi Definitions */

#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIVIDER); // you can change this clock speed

#define WLAN_SSID       "MIT"           // cannot be longer than 32 characters!
#define WLAN_PASS       "ThisIsDummyText"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_UNSEC

#define IDLE_TIMEOUT_MS  3000      // Amount of time to wait (in milliseconds) with no data 
                                   // received before closing the connection.  If you know the server
                                   // you're accessing is quick to respond, you can reduce this value.

// What page to grab!
#define WEBSITE      "droideka.herokuapp.com"
#define WEBPAGE      "/droid"

/**************************************************************************/
/*!
    @brief  Sets up the HW and the CC3000 module (called automatically
            on startup)
*/
/**************************************************************************/

uint32_t ip;
Adafruit_CC3000_Client www;

char line[128];
char c[3];
byte controlData[3];
boolean found;
unsigned long start;


int motorDirections = FORWARD;

Servo tiltServo;

/* Motor and Servo functions */

// note: forward and backward might be switched
void setMotorMode(int mode) {
    if (mode==FORWARD) {
        digitalWrite(motorAm1,HIGH);
        digitalWrite(motorAm2,LOW);
        Serial.println("frwrd");
    } else if (mode==BACKWARD) {
        digitalWrite(motorAm1,LOW);
        digitalWrite(motorAm2,HIGH);
        Serial.println("bkwrd");
    } else if (mode==STOP) {
        digitalWrite(motorAm1,LOW);
        digitalWrite(motorAm2,LOW);
        Serial.println("stop");
    }
}

void setMotorSpeed(int speed) {
    analogWrite(motorPWM,speed);
}

void setTiltServoState(int tiltState) {
  if (tiltState==LEFT) {
        tiltServo.write(0);
    } else if (tiltState==RIGHT) {
        tiltServo.write(180);
    } else if (tiltState==CENTER) {
        tiltServo.write(90);
    }
}

/* Wifi Functions */

//populates control (a length 3 array) with the control codes
//c[0] = 0-255 (speed)
//c[1] = 0,left; 1,right; 2,center
//c[2] = 0,forward; 1,backward
void getControlData(byte *control){

  Serial.println(F("Begin"));  
  Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC);
  Serial.print("Time Since Reset: "); Serial.println(millis()-start);
  www = cc3000.connectTCP(ip, 80);
  
  while (!www.connected()) {
    Serial.println(F("Connection failed"));
    www.close();
    delay(100);
    www = cc3000.connectTCP(ip, 80);  
  }
  www.fastrprint(F("GET "));
  www.fastrprint(WEBPAGE);
  www.fastrprint(F(" HTTP/1.1\r\n"));
  www.fastrprint(F("Host: ")); www.fastrprint(WEBSITE); www.fastrprint(F("\r\n"));
  www.fastrprint(F("\r\n"));
  www.println();
  Serial.println(F("Connected"));
  //} else {
  //  return;
  //}
  
  
  /* Read data until either the connection is closed, or the idle timeout is reached. */ 
  found = 0;
  unsigned long lastRead = millis();
  while (www.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS) && !found) {
    while (www.available() && !found) {
      c[0] = c[1];
      c[1] = c[2];
      c[2] = www.read();
      if(c[0] == '\n' && c[2] == '\n'){
        control[0] = www.read();
        control[1] = www.read();
        control[2] = www.read();
        found = true;
      }
      lastRead = millis();
    }
  }
      
  www.close();
  //Serial.println(F("-------------------------------------"));
  
  /* You need to make sure to clean up after yourself or the CC3000 can freak out */
  /* the next time your try to connect ... */
  //Serial.println(F("\n\nDisconnecting"));
  //cc3000.disconnect();
  
}

void prettyPrintControl(byte *control){
  Serial.print(F("Speed Setting: "));
  Serial.println(control[0]);
  Serial.print(F("Turn Setting: "));
  Serial.println(control[1]==0 ? F("Left") : control[1]==1 ? F("Right") : F("Center"));
  Serial.print(F("Direction: "));
  Serial.println(control[2]==0 ? F("Forward") : F("Backward"));
}

/**************************************************************************/
/*!
    @brief  Begins an SSID scan and prints out all the visible networks
*/
/**************************************************************************/

void listSSIDResults(void)
{
  uint8_t valid, rssi, sec, index;
  char ssidname[33]; 

  index = cc3000.startSSIDscan();

  Serial.print(F("Networks found: ")); Serial.println(index);
  Serial.println(F("================================================"));

  while (index) {
    index--;

    valid = cc3000.getNextSSID(&rssi, &sec, ssidname);
    
    Serial.print(F("SSID Name    : ")); Serial.print(ssidname);
    Serial.println();
    Serial.print(F("RSSI         : "));
    Serial.println(rssi);
    Serial.print(F("Security Mode: "));
    Serial.println(sec);
    Serial.println();
  }
  Serial.println(F("================================================"));

  cc3000.stopSSIDscan();
}

/**************************************************************************/
/*!
    @brief  Tries to read the IP address and other connection details
*/
/**************************************************************************/
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
  
  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}


// run once
void setup() {     
  Serial.begin (115200);
  Serial.println("Start");


  /* Motor and Servo Things */
  
  //pinMode(wifiEnable, OUTPUT);         
  //pinMode(accelCS, OUTPUT);
  //pinMode(wifiCS, OUTPUT);
  pinMode(motorAm1, OUTPUT);
  pinMode(motorAm2, OUTPUT);
  pinMode(motorPWM, OUTPUT);

  //digitalWrite(wifiEnable,HIGH);

  /* Wifi Things */
  
    Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC);

  /* Initialise the module */
  Serial.println(F("\nInitializing..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Couldn't begin()! Check your wiring?"));
    while(1);
  }
  
  // Optional SSID scan
  // listSSIDResults();
  
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }
   
  Serial.println(F("Connected!"));
  
  /* Wait for DHCP to complete */
  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }  

  /* Display the IP address DNS, Gateway, etc. */  
  while (! displayConnectionDetails()) {
    delay(1000);
  }

  ip = 0;

  // Try looking up the website's IP address
  Serial.print(WEBSITE); Serial.print(F(" -> "));
  while (ip == 0) {
    if (! cc3000.getHostByName(WEBSITE, &ip)) {
      Serial.println(F("Couldn't resolve!"));
    }
    delay(500);
  }
  cc3000.printIPdotsRev(ip);


  /* Try connecting to the website.
     Note: HTTP/1.1 protocol is used to keep the server from closing the connection before all data is read.
  */
  
  
  tiltServo.attach(tiltServoPin);


  start = millis();

}

void loop() {
  getControlData(controlData);
  prettyPrintControl(controlData);
  setMotorMode(controlData[2]);
  setMotorSpeed(controlData[0]);
  setTiltServoState(controlData[1]);
  delay(100);    
}
