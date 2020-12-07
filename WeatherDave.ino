#include <WiFiNINA.h>
#include "ThingSpeak.h"
#include "secrets.h"
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11


DHT dht(DHTPIN, DHTTYPE);

char ssid[] = SECRET_SSID;    //  your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;


int temperaturePin = A0;
int lightPin = A1;
int lightValue = 0;
int windPin = A2;
int windVoltage = 0;
int windZero = 590;
int windRatio = 20;


void setup() {
    Serial.begin(115200);

    if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
    }
    
    ThingSpeak.begin(client);

    dht.begin();

    }

void loop() {
  
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  //Read values
  
  float windVoltage = analogRead(windPin);
  float lightValue = analogRead(lightPin);
  
  float windValue = windVoltage- windZero;

  windValue = windValue / windRatio;
  
  float humidityValue = dht.readHumidity();
  float temperatureValue = dht.readTemperature();

  
  Serial.println("Temperatur: ");
  Serial.println(temperatureValue);
  Serial.println("VindVolt: ");
  Serial.println(windValue);
  
  //Write to Thingspeak

  ThingSpeak.setField(1, lightValue);
  ThingSpeak.setField(2, temperatureValue);
  ThingSpeak.setField(3, humidityValue);
  ThingSpeak.setField(4, windValue);

  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel  update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  delay(20000);
 }
 
