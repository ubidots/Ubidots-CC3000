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
4. Select the .ZIP file of Adafruit_CC3000 and then "Accept" or "Choose"
5. Do the same to add the UbidotsCC3000 library.
6. Close the Arduino IDE and open it again.

<aside class="alert">
Use the Arduino 1.6.4 version or newer with the CC3000, EXCEPT for the firmware update sketches in the library examples folder. Use 1.0.6 for those!
</aside>
<aside class="warning">
Make sure your Arduino is powered by a 1 amp or higher rated external power supply when using with the CC3000! Powering an Arduino + CC3000 from a computer/laptop USB port will lead to unstable behavior and lockups because the USB port can't supply enough power!
</aside>

**Note**: The library will create a new Ubidots data source named **CC3000**. If you desire assign a different device name and label, you can add the add to your code the following lines:

* To change the Device Name:

```
client.setDataSourceName("New_name");
```

* To change the Device label:

``` 
client.setDataSourceLabel("New_label"); 
```

## Send values to Ubidots

The following example is to send one value to Ubidots, it will create the variable automatically with the label assign by you on the code. If you desire send more than one value just add this line ```client.add("variable_name", value);``` to your code with the parameters needed. 

Also, you can find the example on the library examples. Go to **Sketch -> Examples -> UbidotsCC3000 library** and select the **"UbidotsSaveValue"** example.

Add your Ubidots TOKEN where indicated, as well as the WI-FI settings.

Upload the code, open the Serial monitor to check the results. If no response is seen, try unplugging your Arduino and then plugging it again. Make sure the baud rate of the Serial monitor is set to the same one specified in your code.

```c++
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
```
## Get value from Ubidots

To get the last value of a variable from Ubidots you can use two different functions:

+ client.getValue(ID)
+ client.getValueWithDatasource(DEVICE_LABEL, VARIABLE_LABEL)

On the example folder you'll find all the examples codes. Go to **Sketch -> Examples ->  UbidotsCC3000 library** and select the one that you need "**UbidotsGetValue**";**UbidotsGetValueWithDevice**".

### client.getValue(ID)

This function let you get de last value of a variable using the variable ID.

Add your Ubidots TOKEN and variable ID where indicated, as well as the WI-FI settings.
```c++
#include <UbidotsCC3000.h>
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>

#define WLAN_SSID       "...."  // Your WiFi SSID, cannot be longer than 32 characters!
#define WLAN_PASS       "$...."  // Replace it with your WiFi pass
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define TOKEN "...."  // Replace it with your Ubidots token
#define ID "...." // Replace it with your Ubidots' variable ID

Ubidots client(TOKEN);

void setup() {
  Serial.begin(115200);
  client.initialize();
  client.wifiConnection(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);
  //client.setDebug(true);//Uncomment this line to set DEBUG on

void loop() {
  float value = client.getValue(ID);
  Serial.print("value: ");
  Serial.println(value);
  delay(5000);
}
```

### client.getValueWithDatasource(DEVICE_LABEL, VARIABLE_LABEL)

This function let you get de last value of a variable using the device and variable labels.

Add your Ubidots TOKEN, device label, and variable label where indicated, as well as the WI-FI settings.

```c++
#include <UbidotsCC3000.h>
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>

#define WLAN_SSID       "...."  // Your WiFi SSID, cannot be longer than 32 characters!
#define WLAN_PASS       "...."  // Replace it with your WiFi pass
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define TOKEN "...."  // Replace it with your Ubidots token
#define DEVICE_LABEL "...." // Replace it with your Ubidots' device label
#define VARIABLE_LABEL "...." // Replace it with your Ubidots' variable label}

Ubidots client(TOKEN);

void setup() {
  Serial.begin(115200);
  client.initialize();
  client.wifiConnection(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);
  //client.setDebug(true);//Uncomment this line to set DEBUG on
}

void loop() {
  float value = client.getValueWithDatasource(DEVICE_LABEL, VARIABLE_LABEL);
  Serial.print("value: ");
  Serial.println(value);
  delay(5000);
}
```
