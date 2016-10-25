/*
Copyright (c) 2013-2016 Ubidots.
Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:
The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Made by Mateo Velez - Metavix for Ubidots Inc
*/

#include "UbidotsCC3000.h"

/**
 * Constructor.
 */
Ubidots::Ubidots(char* token) {
    _token = token;
    _dsName = "CC3000";
    _dsTag = "CC3000";
    currentValue = 0;
    val = (Value *)malloc(MAX_VALUES*sizeof(Value));
}

/** 
 * This function is to get value from the Ubidots API
 * @arg id the id where you will get the data
 * @return num the data that you get from the Ubidots API
 */
float Ubidots::getValueWithDatasource(char* dsTag, char* idName) {
  float num;
  int i = 0;
  char* allData = (char *) malloc(sizeof(char) * 250);
  String response;
  uint8_t bodyPosinit;
  sprintf(allData, "Particle/1.0|LV|%s|%s:%s|end", _token, dsTag, idName);
  _client.connect(ip, PORT);
    if (_client.connected()) {
        Serial.println("Geting your variable");
        _client.fastrprint(allData);
    } else {
        Serial.println(F("Connection failed"));  
        currentValue = 0;  
        return NULL;
    }
    unsigned long lastRead = millis();
    while (_client.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
        while (_client.available()) {
            char c = _client.read();
            response += c;
        }
    }
    bodyPosinit = 3 + response.indexOf("OK|");
    response = response.substring(bodyPosinit);
    num = response.toFloat();
    currentValue = 0;
    _client.stop();
    free(allData);
    return num;
}

/** 
 * This function is to get value from the Ubidots API
 * @arg id the id where you will get the data
 * @return num the data that you get from the Ubidots API
 */
float Ubidots::getValue(char* id) {
	float num;
	int i = 0;
    String raw;
    uint8_t bodyPosinit;
	uint8_t bodyPosend;
	char reply[15];
	_client.connect(ip, PORT);
	if (_client.connected()) {
    	Serial.println("Geting your variable");
        _client.fastrprint(F("CC3000|GET:"));
        _client.fastrprint(_token);
        _client.fastrprint(F("|"));
        _client.fastrprint(id);
        _client.fastrprint(F("|end"));
    } else {
        Serial.println(F("Connection failed"));  
        currentValue = 0;  
        return NULL;
    }
    unsigned long lastRead = millis();
    while (_client.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
        while (_client.available()) {
            reply[i] = _client.read();
            Serial.print(reply[i]);
            lastRead = millis();
            i++;
        }
    }
    Serial.println();
    num = atof(reply); 
    _client.close();
    return num;
}
void Ubidots::add(char *variable_id, float value) {
  add(variable_id, value, NULL);
}
/**
 * Add a value of variable to save
 * @arg variable_id variable id to save in a struct
 * @arg value variable value to save in a struct
 */
void Ubidots::add(char *variable_id, float value, char *ctext1) {
  (val+currentValue)->idName = variable_id;
  (val+currentValue)->idValue = value;
  currentValue++;
  if (currentValue > MAX_VALUES) {
        currentValue = MAX_VALUES;
  }
}
/**
 * Send all data of all variables that you saved
 * @reutrn true upon success, false upon error.
 */
bool Ubidots::sendAll() {
    int i;
    String allData;
    if (_dsName == "CC3000") {
        allData = USER_AGENT; 
        allData += "|POST|";
        allData += _token;
        allData += "|";
        allData += _dsTag;
        allData += "=>";
        //sprintf(allData, "%s|POST|%s|%s=>", USER_AGENT, _token, _dsTag);
    } else {
        allData = USER_AGENT; 
        allData += "|POST|";
        allData += _token;
        allData += "|";
        allData += _dsTag;
        allData += ":";
        allData += _dsName;
        allData += "=>";
        //sprintf(allData, "%s|POST|%s|%s:%s=>", USER_AGENT, _token, _dsTag, _dsName);
    }
    for (i = 0; i < currentValue; ) {
        allData += (val + i)->idName;
        allData += ":";
        allData += String((val + i)->idValue,2);
        //sprintf(allData, "%s%s:%f", allData, (val + i)->idName, (val + i)->idValue);
        i++;
        if (i < currentValue) {
            allData += ",";
            //sprintf(allData, "%s,", allData);
        }
    }
    allData += "|end";
    //sprintf(allData, "%s|end", allData);
#ifdef DEBUG_UBIDOTS
    Serial.println(allData);
#endif
    _client.connect(ip, PORT);
    if (_client.connected()) {        // Connect to the server
#ifdef DEBUG_UBIDOTS
        Serial.println("Client connected"); 
#endif
        _client.print(allData);
        Serial.println("envie");
    } else {
        // if you couldn't make a connection:
        Serial.println("connection failed");
        Serial.println();
        Serial.println("disconnecting.");
        currentValue = 0;
        _client.stop();
        _client.close();
        return false;
    }
    while (!_client.available());
    unsigned long lastRead = millis();
    while (_client.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
        while (_client.available()) {
            char c = _client.read();
            Serial.print(c);
            lastRead = millis();
        }
    }
    Serial.println();
    _client.stop();
    _client.close();
    currentValue = 0;
    //free(allData);
    return true;
}

/**
 * Initialize the cc3000 WiFi
 */
void Ubidots::initialize() {
    Serial.println(F("\nInitializing..."));
    if (!cc3000.begin()) {
        Serial.println(F("Couldn't begin()! Check your wiring?"));
        while(1);
    }
}
/**
 * Connects to the WiFi service
 * @arg ssid the ssid of the WiFi
 * @arg pass the pass of the WiFi
 * @arg security 
 */
void Ubidots::wifiConnection(char* ssid, char* pass, uint8_t security) {
    Serial.print(F("\nAttempting to connect to ")); Serial.println(ssid);
    if (!cc3000.connectToAP(ssid, pass, security)) {
        Serial.println(F("Failed!"));
        while(1);
    }
    Serial.println(F("Connected!"));

    /* Wait for DHCP to complete */
    Serial.println(F("Request DHCP"));
    while (!cc3000.checkDHCP()){
        delay(100); // ToDo: Insert a DHCP timeout!
    }
    getIpWebSite();
    _client = cc3000.connectTCP(ip, PORT);
    _client.close();
}
/**
 * Get the IP of the website
 */
void Ubidots::getIpWebSite(){
    ip = 0;
    // Try looking up the website's IP address
    while (ip == 0) {
    if (! cc3000.getHostByName(WEBSITE, &ip)) {
        Serial.println(F("Couldn't resolve!"));
    }
        delay(500);
    }
    cc3000.printIPdotsRev(ip);
    Serial.println();
}
