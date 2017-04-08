#include <UbidotsCC3000.h>
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>

#define WLAN_SSID       "...."  // Your WiFi SSID, cannot be longer than 32 characters!
#define WLAN_PASS       "....."  // Replace it with your WiFi pass
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define TOKEN "...."  // Replace it with your Ubidots token
#define VARIABLE_LABEL "...." // Assign the variable label


Ubidots client(TOKEN);

void setup() {
  Serial.begin(115200);
  client.initialize();
  client.wifiConnection(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);
  //client.setDebug(true);//Uncomment this line to set DEBUG on
}

void loop() {
  float value = analogRead(A0);
  client.add(VARIABLE_LABEL, value);
  client.sendAll();
  delay(5000);
}
