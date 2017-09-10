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
4. Select the .ZIP file of Ubidots Arduino CC3000 and then "Accept" or "Choose"
5. Do the same to add the Adafruit CC3000 library.
6. Close the Arduino IDE and open it again.

<aside class="alert">
Use the Arduino 1.6.4 version or newer with the CC3000, EXCEPT for the firmware update sketches in the library examples folder. Use 1.0.6 for those!
</aside>

<aside class="warning">
Make sure your Arduino is powered by a 1 amp or higher rated external power supply when using with the CC3000! Powering an Arduino + CC3000 from a computer/laptop USB port will lead to unstable behavior and lockups because the USB port can't supply enough power!
</aside>

## Change the default name

The library will create a new Ubidots device named **"Arduino-CC3000"**, also assigns the same name for the device label. if you desire to assign a differente device label, please add to your `setup()` function the line below:

> client.setDeviceLabel("my-new-device")

## Send values to Ubidots

To send onen value to Ubidots, go to **Sketch -> Examples -> ubidots-arduino-cc3000 library** and select the **"UbidotsSaveValue"** example.

Update your WiFi crendentials, assign your Ubidots [TOKEN](http://help.ubidots.com/user-guides/find-your-token-from-your-ubidots-account) and the variable label where is indicated.

Once the parameters are assigned, upload the code. Then, open the Serial monitor to check the results.

**NOTE**: If no response is seen, try unplugging your Arduino and then plugging it again. Make sure the baud rate of the Serial monitor is set to the same one specified in your code.

```c++
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
#define VARIABLE_LABEL "temperature" // Assign the Ubidots variable label

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
   /* Sensors readings */
  float value = analogRead(A0);
 /* Sending value to Ubidots */
  client.add(VARIABLE_LABEL,value);
  client.sendAll();
  delay(5000);
}

```

The library allow you send just **5 variables** per request. But don't worry for that, if you desire send more than 5 variables, go to **Sketch -> Examples -> ubidots-arduino-cc3000 library** and select the **"UbidotsSaveMultiValues"** example. Don't forget update your WiFi credentials and assign your Ubidots [TOKEN](http://help.ubidots.com/user-guides/find-your-token-from-your-ubidots-account), and the variables labels where is indicated.

## Get one value from Ubidots

To get the last value of a variable from Ubidots, go to **Sketch -> Examples -> ubidots-arduino-cc3000 library** and select the **"UbidotsGetValue"** example.

Update your WiFi crendentials, assign your Ubidots [TOKEN](http://help.ubidots.com/user-guides/find-your-token-from-your-ubidots-account), the device and variable label from where you desire obtain the value.

Upload the code, open the Serial monitor to check the results.

**NOTE**: If no response is seen, try unplugging your Arduino and then plugging it again. Make sure the baud rate of the Serial monitor is set to the same one specified in your code.

```c++
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
#define DEVICE_LABEL "Your_id_here" // Replace it with your Ubidots' device label
#define VARIABLE_LABEL "Your_id_here" // Replace it with your Ubidots' variable label

/* initialize the instance */
Ubidots client(TOKEN);

/********************************
 * Main Functions
 *******************************/
void setup() {
  Serial.begin(115200);
  client.initialize();
  client.wifiConnection(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);
  //client.setDebug(true); // Uncomment this line to set DEBUG on
}

void loop() {
  /* Getting the last value from a variable */
  float value = client.getValue(DEVICE_LABEL, VARIABLE_LABEL);
  /* Print the value obtained */
  Serial.print("The value obteined is: ");
  Serial.println(value);
  delay(5000);
}
```
