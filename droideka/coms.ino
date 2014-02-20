#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"

// WiFi Constants
#define WLAN_SSID       "MIT"  
#define WLAN_PASS       "ThisIsDummyText"
#define WLAN_SECURITY   WLAN_SEC_UNSEC

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11

Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIVIDER); // you can change this clock speed

// Amount of time to wait (in milliseconds) with no data 
// received before closing the connection.  If you know the server
#define IDLE_TIMEOUT_MS  3000

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


//populates control (a length 3 array) with the control codes
//c[0] = 0-255 (speed)
//c[1] = 0,left; 1,right; 2,center
//c[2] = 0,forward; 1,backward
void getControlData(byte *control){
  char c[3];
  boolean found = 0;

  //Connect to the website
  Serial.println(F("Attempting Connection"));
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
  
  

  //Grab the first 3 bytes after the double enter

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
  
}

void prettyPrintControl(byte *control){
  Serial.print(F("Speed Setting: "));
  Serial.println(control[0]);
  Serial.print(F("Turn Setting: "));
  Serial.println(control[1]==0 ? F("Left") : control[1]==1 ? F("Right") : F("Center"));
  Serial.print(F("Direction: "));
  Serial.println(control[2]==0 ? F("Forward") : F("Backward"));
}

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

void setupWifi(){
  /* Initialise the module */
  Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC);
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
}