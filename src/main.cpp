#include <Arduino.h>
#include <functional>
#include "FS.h"


#include <ESP8266WiFi.h>

#include <SensorManager.h>

 
const char* ssid = "KAERVEJ.IOT";
const char* password =  "HomeAutomationKaervej";
 


class SensorManager;
SensorManager sensorMgr = SensorManager();

void setup() {
  //Serial.begin(115200);
  Serial.begin(9600);
 
 //connectionHandlers(LinkedList<AsyncWebSocketClient *>([](AsyncWebSocketClient *h){ delete h; }))

  if(!SPIFFS.begin()){
     Serial.println("An Error has occurred while mounting SPIFFS");
     return;
  }
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println(WiFi.localIP());
 


  //SensorManager *pMgr = new SensorManager();
  // put your setup code here, to run once:
  sensorMgr.setup();
  
}

void loop() {
  
  sensorMgr.loop();
}