/*
Copyright (c) 2017, Ubidots.
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

Original Maker: Mateo Velez - Metavix for Ubidots Inc
Modified and Maintened by: María Carlina Hernández ---- Developer at Ubidots Inc
                           https://github.com/mariacarlinahernandez
                           Jose Garcia ---- Developer at Ubidots Inc
                           https://github.com/jotathebest
*/

#include "UbidotsCC3000.h"

/**
 * Constructor.
 * Default deviceLabel is "arduino-cc3000"
 */
Ubidots::Ubidots(const char* token, const char* server) {
    _token = token;
    _server = server;
    maxValues = 5;
    currentValue = 0;
    _deviceLabel = DEFAULT_DEVICE_LABEL;
    val = (Value *)malloc(maxValues*sizeof(Value));
}

/***************************************************************************
FUNCTIONS TO RETRIEVE DATA
***************************************************************************/

/**
 * This function is to get last value from the Ubidots API with the device label
 * and variable label
 * @arg device_label is the label of the device
 * @arg variable_label is the label of the variable
 * @return num the data that you get from the Ubidots API, if any error occurs
    the function returns ERRORs messages
 */
float Ubidots::getValue(char* device_label, char* variable_label) {

  /* Assigns the constans as global on the function */
  bool flag = false;
  char* res = (char *) malloc(sizeof(char) * 250);
  char* value = (char *) malloc(sizeof(char) * 30);
  float num;
  uint8_t index = 0;
  uint8_t l = 0;
  uint8_t timeout = 0;
  uint8_t max_retries = 0;

  /* Initial Connection */
  _client.connect(ip, PORT);
  /* Reconnect the client when is disconnected */
  while (!_client.connected()) {
    if (_debug) {
      Serial.println(F("Attemping to connect"));
    }

    if (_client.connect(SERVER, PORT)) {
      break;
    }

    max_retries++;
    if (max_retries > 5) {
      if (_debug) {
        Serial.println(F("Could not connect to server"));
      }
      return ERROR_VALUE;
    }
    delay(5000);
  }

  if(_debug){
    Serial.println(F("Making the HTTP request..."));
  }
  /* Make the HTTP request to the server*/
  _client.fastrprint(F("GET /api/v1.6/devices/"));
  _client.fastrprint(device_label);
  _client.fastrprint(F("/"));
  _client.fastrprint(variable_label);
  _client.fastrprint(F("/lv HTTP/1.1\r\n"));
  _client.fastrprint(F("Host: things.ubidots.com\r\n"));
  _client.fastrprint(F("User-Agent: "));
  _client.fastrprint(USER_AGENT);
  _client.fastrprint(F("/"));
  _client.fastrprint(VERSION);
  _client.fastrprint(F("\r\n"));
  _client.fastrprint(F("X-Auth-Token: "));
  _client.fastrprint(_token);
  _client.fastrprint(F("\r\n"));
  _client.fastrprint(F("Connection: close\r\n"));
  _client.println();
  /* Reach timeout when the server is unavailable */
  while (!_client.available() && timeout < 2000) {
    timeout++;
    delay(1);
    if (timeout >= 2000) {
      if (_debug) {
        Serial.println(F("Error, max timeout reached"));
      }
      _client.stop();
      return ERROR_VALUE;
    }
  }

  /* Reads the response from the server */
  while (_client.available()) {
    char c = _client.read();
    if (_debug) {
      Serial.write(c);
    }
    if (c == -1) {
      if (_debug) {
        Serial.println(F("Error reading data from server"));
      }
      _client.stop();
      return ERROR_VALUE;
    }
    res[l++] = c;
  }

  /* Loop the server response */
  for (int i = 0; i<dataLen(res)-4; i++) {
    if ((res[i+0] == '\r') && (res[i+1] == '\n') && (res[i+2] == '\r') && (res[i+3] == '\n')) {
      i = i+5;
      for (int j = i; j < dataLen(res)-4; j++) {
        /* Loop the server response to obtain the last value */
        if ((res[j+0] == '\r') && (res[j+1] == '\n')) {
          if (flag) {
            break;
          }
          j = j+2;
          flag = true;
        }
        /* Save the value obtained */
        if (flag) {
          value[index] = res[j];
          index++;
          /* Verify when the value received finish */
          if (res[j + 1] == '\r') {
            break;
          }
        }
      }
    }
  }

  /* Converts the value obtained to a float */
  num = atof(value);
  /* Free memory*/
  free(res);
  free(value);
  /* Removes any buffered incoming serial data */
  _client.flush();
  /* Disconnects the client */
  _client.stop();
  return num;
 }


/***************************************************************************
FUNCTIONS TO SEND DATA
***************************************************************************/

/**
 * Add a value of variable to save
 * @arg variable_label variable label to save in a struct
 * @arg value variable value to save in a struct
 * @arg ctext [optional] is the context that you will save, default
 * @arg timestamp_val [optional] is the timestamp for the actual value
 * is NULL
 */
void Ubidots::add(const char * variable_label, double value) {
 return add(variable_label, value, '\0', '\0');
}

void Ubidots::add(const char * variable_label, double value, char* ctext) {
 return add(variable_label, value, ctext, '\0');
}

void Ubidots::add(const char * variable_label, double value, char* ctext, long unsigned timestamp_val ) {
 (val+currentValue)->varLabel = variable_label;
 (val+currentValue)->varValue = value;
 (val+currentValue)->context = ctext;
 (val+currentValue)->timestamp_val = timestamp_val;
 currentValue++;
 if (currentValue>maxValues) {
   Serial.println(F("You are sending more than 5 consecutives variables, you just could send 5 variables. Then other variables will be deleted!"));
   currentValue = maxValues;
 }
}

/**
 * Send all data of all variables that you saved
 * @reutrn true upon success, false upon error.
 */
bool Ubidots::sendAll() {
  /* Assigns the constans as global on the function */
  char body_lenght[3];
  uint8_t length;
  uint8_t max_retries = 0;
  uint8_t timeout = 0;
  uint8_t i = 0;
  String str;
  /* Verify the variables invoked */
  if (currentValue == 0) {
    Serial.println(F("Invoke a variable to be send using the method \"add\""));
    return false;
  }
  /* Saves variable value in str */
  str = String(((val+i)->varValue),2); // variable value
  /* Builds the JSON to be send */
  //char* body = (char *) malloc(sizeof(char) * 150);
  //sprintf(body, "{");
  String body;
  body += "{";
  for (i = 0; i < currentValue;) {
    //sprintf(body, "%s\"%s\":", body, (val + i)->varLabel);
    body += "\"";
    body += (val + i)->varLabel;
    body += "\": ";
    if ((val + i)->context != '\0') {
      //sprintf(body, "%s{\"value\":%s, \"context\":{%s}}", body, str.c_str(), (val + i)->context);
      body += "{\"value\": ";
      body += str.c_str();
      body += ", \"context\":{";
      body += (val + i)->context;
      body += "}}";
    } else if ((val + i)-> timestamp_val != '\0') {
      //sprintf(body, "%s{\"value\":%s, \"timestamp\":%s}", body, str.c_str(), (val + i)->timestamp_val);
      body += "{\"value\":";
      body += str.c_str();
      body += ", \"timestamp\":";
      body += (val + i)->timestamp_val;
      body += "}";
    } else {
      //sprintf(body, "%s%s", body, str.c_str());
      body += str.c_str();
    }
    i++;
    if (i < currentValue) {
      //sprintf(body, "%s, ", body);
      body +=  ",";
    }
  }
  //sprintf(body, "%s}", body);
  body += "}";
  if (_debug) {
    Serial.print("the body to be sent is: ");
    Serial.println(body);
  }
  /* Save the length of the body */
  length = body.length();
  sprintf(body_lenght, "%d", length);

  /* Initial connection */
  _client.connect(ip,PORT);
  /* Reconnect the client when is disconnected */
  while (!_client.connected()) {
    if (_debug) {
      Serial.println(F("Attemping to connect"));
    }
    if (_client.connect(ip, PORT)) {
      break;
    }
    max_retries++;
    if (max_retries > 5) {
      if (_debug) {
        Serial.println(F("Could not connect to server"));
      }
      return ERROR_VALUE;
    }
    delay(5000);
  }

  if (_debug) {
    Serial.println(F("Making the HTTP request...\r\n"));
  }
  /* Builds and make the HTTP request to the server
  Please reference this link to know all the request's structures https://ubidots.com/docs/api/ */
  _client.fastrprint(F("POST /api/v1.6/devices/"));
  _client.fastrprint(_deviceLabel);
  _client.fastrprint(F(" HTTP/1.1\r\n"));
  _client.fastrprint(F("Host: things.ubidots.com\r\n"));
  _client.fastrprint(F("User-Agent: "));
  _client.fastrprint(USER_AGENT);
  _client.fastrprint(F("/"));
  _client.fastrprint(VERSION);
  _client.fastrprint(F("\r\n"));
  _client.fastrprint(F("X-Auth-Token: "));
  _client.fastrprint(_token);
  _client.fastrprint(F("\r\n"));
  _client.fastrprint(F("Connection: close\r\n"));
  _client.fastrprint(F("Content-Type: application/json\r\n"));
  _client.fastrprint(F("Content-Length: "));
  _client.fastrprint(body_lenght);
  _client.fastrprint(F("\r\n\r\n"));
  _client.println(body);
  _client.fastrprint(F("\r\n\r\n"));
  /* Reach timeout when the server is unavailable */
  while (!_client.available() && timeout < 5000) {
    timeout++;
    delay(1);
    if (timeout >= 5000) {
      if (_debug) {
        Serial.println(F("Error, max timeout reached"));
      }
      _client.stop();
      return ERROR_VALUE;
    }
  }
  /* Reads the response from the server */
  while (_client.available()) {
    char c = _client.read();
    Serial.write(c);
    if (c == -1) {
      if (_debug) {
        Serial.println(F("Error reading data from server"));
      }
      _client.stop();
      return ERROR_VALUE;
    }
  }
  currentValue = 0;
  /* Removes any buffered incoming serial data */
  _client.flush();
  /* Disconnects the client */
  _client.stop();
  return true;
}

/***************************************************************************
AUXILIAR FUNCTIONS
***************************************************************************/

/**
 * Assigns a new device label
 * @arg new_device_name new label that you want to assign to your device
 */
void Ubidots::setDeviceLabel(const char * new_device_label) {
   _deviceLabel = new_device_label;
}

/**
 * Gets the length of a variable
 * @arg variable a variable of type char
 * @return dataLen the length of the variable
 */
int Ubidots::dataLen(char* variable) {
  uint8_t dataLen = 0;
  for (int i = 0; i <= 250; i++) {
    if (variable[i] != '\0') {
      dataLen++;
    } else {
      break;
    }
  }
  return dataLen;
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
 * Get the IP of the server
 */
void Ubidots::getIpServer(){
  ip = 0;
  /* Try looking up the website's IP address */
  while (ip == 0) {
    if (! cc3000.getHostByName(SERVER, &ip)) {
      Serial.println(F("Couldn't resolve!"));
    }
    delay(500);
  }
  cc3000.printIPdotsRev(ip);
  Serial.println();
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
  getIpServer();
  _client = cc3000.connectTCP(ip, PORT);
  _client.close();
}

/**
 * Turns on or off debug messages
 * @debug is a bool flag to activate or deactivate messages
 */
void Ubidots::setDebug(bool debug) {
  _debug = debug;
}
