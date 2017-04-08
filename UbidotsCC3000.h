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
Modified by Maria Carlina Hernandez for Ubidots Inc
*/

#ifndef __UbidotsCC3000_
#define __UbidotsCC3000_

#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11

#define IDLE_TIMEOUT_MS  3000      // Amount of time to wait (in milliseconds) 
#define PORT 9012
////////////////////////////////////// Ubidots parameters
#define WEBSITE "translate.ubidots.com"
#define USER_AGENT "CC3000"
#define VERSION "1.1"

#define MAX_VALUES 3

typedef struct Value {
  char  *idName;
  char  *contextOne;
  int idValue;
  unsigned long timestamp_val;
} Value;

class Ubidots{
 private:
        Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
        SPI_CLOCK_DIVIDER); // you can change this clock speed
        char* _token;
        char* _dsLabel;
        char* _dsName;
        uint8_t maxValues;
        uint8_t currentValue;
        Value * val;
        uint32_t ip;
        void getIpWebSite();
        bool _debug = false;
        Adafruit_CC3000_Client _client;
 public:
        Ubidots(char* token);
        void initialize();
        void setDebug(bool debug);
        void setDataSourceName(char* dsName);
<<<<<<< HEAD
        void setDataSourceLabel(char* dsLabel);
=======
        void setDataSourceLabel(char* dsTag);

>>>>>>> 223e710c336afd6c76fcfde0735bf3eea04adf1e
        float getValue(char* id);
        void add(char *variable_id, int value);
        void add(char *variable_id, int value, char *ctext);
        void add(char *variable_id, int value, char *ctext, unsigned long timestamp);
        void wifiConnection(char* ssid, char* pass, uint8_t security);
        bool sendAll();
        float getValueWithDatasource(char* dsName, char* idName);
};
#endif