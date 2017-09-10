/********************************
 * Libraries included
 *******************************/
#include <UbidotsCC3000.h>
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>

/********************************
 * Constants and objects
 *******************************/
#define WLAN_SSID       "OpenWRT"  // Your WiFi SSID, cannot be longer than 32 characters!
#define WLAN_PASS       "Your_pass_here"  // Replace it with your WiFi pass
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2
/* Assigns the Ubidots parameters */
#define TOKEN "Your_token_here"  // Replace it with your Ubidots token
#define VARIABLE_LABEL "position" // Assign the Ubidots variable label

/* initialize the instance */
Ubidots client(TOKEN);

/********************************
 * Main Functions
 *******************************/
void setup() {
  Serial.begin(115200);
  client.initialize();
  client.wifiConnection(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);
  //client.setDeviceLabel("my-new-device"); // Uncomment this line to change the default device name
  //client.setDebug(true); // Uncomment this line to set DEBUG on
}

void loop() {
  /* Sends latitude and longitude for watching position in a map */
  char context[25];
  sprintf(context, "\"lat\":1.23, \"lng\":132.12");
  client.add(VARIABLE_LABEL, 1, context);
  client.sendAll();
  delay(5000)
}
