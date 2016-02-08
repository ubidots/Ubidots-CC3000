# Ubidots CC3000 library

<div style="text-align:center "><img src ="https://learn.adafruit.com/system/guides/images/000/000/358/medium800/2013_09_02_IMG_2112-1024.jpg?1448301402 " /></div>
Adafruit cc3000 WiFi shield uses SPI for communication, so you can push data as fast as you want. It has a proper interrupt system with IRQ pin so you can have asynchronous connections. It supports 802.11b/g, open/WEP/WPA/WPA2 security, TKIP & AES. A built in TCP/IP stack with a "BSD socket" interface. 
Here you will learn how to send a multiple variables to the Ubidots API and how to get the last value of variable from Ubidots with CC3000 shield. 

## Requirements

* [An Arduino UNO](http://arduino.cc/en/Main/arduinoBoardUno)
* [A Adafruit CC3000 WiFi Shield](https://www.adafruit.com/product/1491)
* [Arduino IDE 1.6.4 or higher](https://www.arduino.cc/en/Main/Software)
* [Adafruit CC3000 library](https://github.com/adafruit/Adafruit_CC3000_Library/archive/1.0.3.zip)
* [Ubidots CC3000 library](https://github.com/ubidots/Ubidots-CC3000/archive/master.zip)
 
## Setup

1. Download the Adafruit_CC3000 library [here](https://github.com/adafruit/Adafruit_CC3000_Library/archive/1.0.3.zip).
2. Download the UbidotsCC3000 library [here](https://github.com/ubidots/Ubidots-CC3000/archive/master.zip)
3. Go to the Arduino IDE, click on **Sketch -> Include Library -> Add .ZIP Library**
4. Select the .ZIP file of Ubidots_FONA and then "Accept" or "Choose"
5. Do the same to add the Adafruit_FONA library.
6. Close the Arduino IDE and open it again.

<aside class="alert">
Use the Arduino 1.6.4 version or newer with the CC3000, EXCEPT for the firmware update sketches in the library examples folder. Use 1.0.6 for those!
</aside>
<aside class="warning">
Make sure your Arduino is powered by a 1 amp or higher rated external power supply when using with the CC3000! Powering an Arduino + CC3000 from a computer/laptop USB port will lead to unstable behavior and lockups because the USB port can't supply enough power!
</aside>
    
## Send one value to Ubidots

To send a value to Ubidots, go to **Sketch -> Examples -> Ubidots CC3000 library** and select the "saveValue" example. 
Put your Ubidots token and variable ID where indicated, as well as the WiFi settings.
Upload the code, open the Serial monitor to check the results. If no response is seen, try unplugging your Arduino and then plugging it again. Make sure the baud rate of the Serial monitor is set to the same one specified in your code.

```c++
#include <UbidotsCC3000.h>
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
//#include "utility/debug.h"  // Uncomment this line to set DEBUG on

#define WLAN_SSID       "OpenWRT"  // Your WiFi SSID, cannot be longer than 32 characters!
#define WLAN_PASS       "Your_pass_here"  // Replace it with your WiFi pass
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define TOKEN "Your_token_here"  // Replace it with your Ubidots token
#define ID "Your_id_here" // Replace it with your Ubidots' variable ID

Ubidots client(TOKEN);

void setup() {
  Serial.begin(115200);
  client.initialize();
  client.wifiConnection(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);

}

void loop() {
  float value = analogRead(A0);
  client.add(ID,value);
  client.sendAll();
}
```


## Get one value from Ubidots

To get the last value of a variable from Ubidots, go to **Sketch -> Examples -> Ubidots CC3000 library** and select the "getValue" example. 
Put your Ubidots token and variable ID where indicated, as well as the WiFi settings.
Upload the code, open the Serial monitor to check the results. If no response is seen, try unplugging your Arduino and then plugging it again. Make sure the baud rate of the Serial monitor is set to the same one specified in your code.

```c++
#include <UbidotsCC3000.h>
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
//#include "utility/debug.h"  // Uncomment this line to set DEBUG on

#define WLAN_SSID       "OpenWRT"  // Your WiFi SSID, cannot be longer than 32 characters!
#define WLAN_PASS       "Your_pass_here"  // Replace it with your WiFi pass
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define TOKEN "Your_token_here"  // Replace it with your Ubidots token
#define ID "Your_id_here" // Replace it with your Ubidots' variable ID

Ubidots client(TOKEN);

void setup() {
  Serial.begin(115200);
  client.initialize();
  client.wifiConnection(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);

}

void loop() {
  float value = client.getValue(ID);
}
```

## Send multiple values to Ubidots 

To send multiple values to Ubidots, go to **Sketch -> Examples -> Ubidots CC3000 library** and select the "saveValues" example. 
Put your Ubidots token and variable ID's where indicated, as well as the WiFi settings.
Upload the code, open the Serial monitor to check the results. If no response is seen, try unplugging your Arduino and then plugging it again. Make sure the baud rate of the Serial monitor is set to the same one specified in your code.

```c++
#include <UbidotsCC3000.h>
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
//#include "utility/debug.h"  // Uncomment this line to set DEBUG on

#define WLAN_SSID       "OpenWRT"  // Your WiFi SSID, cannot be longer than 32 characters!
#define WLAN_PASS       "Your_pass_here"  // Replace it with your WiFi pass
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define TOKEN "Your_token_here"  // Replace it with your Ubidots token
#define ID "Your_id_here" // Replace it with your Ubidots' variable ID
#define ID2 "Your_id2_here" 
#define ID3 "Your_id3_here"
#define ID4 "Your_id4_here" 
#define ID5 "Your_id5_here"

Ubidots client(TOKEN);

void setup() {
  Serial.begin(115200);
  client.initialize();
  client.wifiConnection(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);

}

void loop() {
  float value = analogRead(A0);
  float value2 = analogRead(A1);
  float value3 = analogRead(A2);
  float value4 = analogRead(A3);
  float value5 = analogRead(A4);
  client.add(ID,value);
  client.add(ID2,value2);
  client.add(ID3,value3);
  client.add(ID4,value4);
  client.add(ID5,value5);
  client.sendAll();
}
```