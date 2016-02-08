#ifndef __UbidotsCC3000_
#define __UbidotsCC3000_
#define DEBUG_UBIDOTS

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
#define PORT 9001
////////////////////////////////////// Ubidots parameters
#define WEBSITE "translate.ubidots.com"

typedef struct Value {
  char  *id;
  float value_id;
} Value;

class Ubidots{
    private:
        Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
        SPI_CLOCK_DIVIDER); // you can change this clock speed
        char* _token;
        uint8_t maxValues;
        uint8_t currentValue;
        Value * val;
        uint32_t ip;
        void getIpWebSite();
        Adafruit_CC3000_Client _client;         

    public:
        Ubidots(char* token);
        void initialize();
        float getValue(char* id);
        void add(char *variable_id, double value);
        void wifiConnection(char* ssid, char* pass, uint8_t security);
        bool sendAll();        
};
#endif