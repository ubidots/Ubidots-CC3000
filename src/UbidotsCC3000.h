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

#ifndef __UbidotsCC3000_
#define __UbidotsCC3000_

#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>

namespace {
  const char * DEFAULT_DEVICE_LABEL = "arduino-cc3000";
  const char * SERVER = "things.ubidots.com";
  const char * USER_AGENT = "Arduino-CC3000";
  const char * VERSION = "2.0";
  const int PORT = 80;
  const float ERROR_VALUE = -3.4028235E+8;
}

typedef struct Value {
  const char * varLabel;
  char *context;
  double varValue;
  unsigned long timestamp_val;
} Value;

class Ubidots{
  private:
    // These are the interrupt and control pins
    uint8_t ADAFRUIT_CC3000_IRQ = 3; // MUST be an interrupt pin!
    // These can be any two pins
    uint8_t ADAFRUIT_CC3000_VBAT = 5;
    uint8_t ADAFRUIT_CC3000_CS = 10;
    // Use hardware SPI for the remaining pins
    // On an UNO, SCK = 13, MISO = 12, and MOSI = 11
    Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
    SPI_CLOCK_DIVIDER); // you can change this clock speed
    Adafruit_CC3000_Client _client;
    bool _debug = false;
    const char * _deviceLabel;
    const char * _token;
    const char * _server;
    int _port;
    int dataLen(char* variable);
    uint8_t maxValues;
    uint8_t currentValue;
    uint32_t ip;
    Value * val;
    void getIpServer();

  public:
    explicit Ubidots(const char * token, const char * server = SERVER);
    void initialize();
    void setDebug(bool debug);
    float getValue(char* device_label, char* variable_label);
    void wifiConnection(char* ssid, char* pass, uint8_t security);
    void add(const char * variable_label, double value);
    void add(const char * variable_label, double value, char* ctext);
    void add(const char * variable_label, double value, char* ctext, unsigned long timestamp);
    void setDeviceLabel(const char * new_device_label);
    bool sendAll();
};
#endif
