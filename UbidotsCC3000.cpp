#include "UbidotsCC3000.h"

/**
 * Constructor.
 */
Ubidots::Ubidots(char* token){
    _token = token;
    maxValues = 4;
    currentValue = 0;
    val = (Value *)malloc(maxValues*sizeof(Value));
}
/** 
 * This function is to get value from the Ubidots API
 * @arg id the id where you will get the data
 * @return num the data that you get from the Ubidots API
 */
float Ubidots::getValue(char* id){
	float num;
	int i = 0;
    String raw;
    uint8_t bodyPosinit;
	uint8_t bodyPosend;
	char reply[15];
	_client.connect(ip, PORT);
	if (_client.connected()) {
    	Serial.println("Geting your variable");
        _client.fastrprint(F("CC3000:GET:"));
        _client.fastrprint(_token);
        _client.fastrprint(F(":"));
        _client.fastrprint(id);
        _client.fastrprint(F(":CC3000"));
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
/**
 * Add a value of variable to save
 * @arg variable_id variable id to save in a struct
 * @arg value variable value to save in a struct
 */
void Ubidots::add(char *variable_id, double value){
  (val+currentValue)->id = variable_id;
  (val+currentValue)->value_id = value;
  currentValue++;
  if(currentValue>maxValues){
  	Serial.println("You are sending more than 5 consecutives variables, you just could send 5 variables. Then other variables will be deleted!");
  	currentValue = maxValues;
  }
}
/**
 * Send all data of all variables that you saved
 * @reutrn true upon success, false upon error.
 */
bool Ubidots::sendAll(){
    int i;
    char vals[10];
    _client.connect(ip, PORT);
    // Next for is to calculate the lenght of the data that you will send
    if (_client.connected()) {
    	Serial.println("Sending your variables");
        _client.fastrprint(F("CC3000:POST:"));
        _client.fastrprint(_token);
        _client.fastrprint(F(":"));
        for(i=0;i<currentValue;){
             dtostrf((val+i)->value_id,10, 5, vals);     
            _client.fastrprint((val+i)->id);
            _client.fastrprint(",");
            _client.fastrprint(vals);
            _client.fastrprint("&"); 
        i++;
        }
        _client.fastrprint(F(":CC3000"));
    } else {
        Serial.println(F("Connection failed"));  
        currentValue = 0;  
        return false;
    }

    /* Read data until either the connection is closed, or the idle timeout is reached. */ 

    unsigned long lastRead = millis();
    while (_client.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
        while (_client.available()) {
            char c = _client.read();
            Serial.print(c);
            lastRead = millis();
        }
    }
    Serial.println();
    _client.close();
    currentValue = 0;
    return true;
}
/**
 * Initialize the cc3000 WiFi
 */
void Ubidots::initialize(){
    Serial.println(F("\nInitializing..."));
    if (!cc3000.begin())
    {
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
void Ubidots::wifiConnection(char* ssid, char* pass, uint8_t security){
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
