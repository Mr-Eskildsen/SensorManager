#include <Arduino.h>
#include <functional>
#include "FS.h"


#include <ESP8266WiFi.h>

#include <SensorManager.h>

#include <SimpleArray.h>
#include <Sensor/BinarySensor.h>
 
const char* ssid = "KAERVEJ.IOT";
const char* password =  "HomeAutomationKaervej";
 


class SensorManager;
//SensorManager sensorMgr = SensorManager();

void setup() {
  Serial.begin(9600);
  SensorManager::create();

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
 
  uint32_t heap1 = 0;
  uint32_t heap2 = 0;
  uint32_t heap3 = 0;
  uint32_t heap4 = 0; 
  
  SimpleArray<BinarySensor*> test = SimpleArray<BinarySensor*>();
  heap1 = ESP.getFreeHeap();
  test.add(new BinarySensor("","",1));
  test.add(new BinarySensor("","",2));
  test.add(new BinarySensor("","",3));
  test.add(new BinarySensor("","",4));
  test.add(new BinarySensor("","",5));
  heap2 = ESP.getFreeHeap();
  test.remove(2);
  test.remove(0);

  
  
  heap3 = ESP.getFreeHeap();
  test.clear();
  heap4 = ESP.getFreeHeap();

  Serial.print("Heap1: ");
  Serial.println(heap1);
  Serial.print("Heap2: ");
  Serial.println(heap2);
  Serial.print("Heap3: ");
  Serial.println(heap3);
  Serial.print("Heap4: ");
  Serial.println(heap4);

  //SensorManager *pMgr = new SensorManager();
  // put your setup code here, to run once:
  SensorManager::setup();
  
}

void loop() {
  
  SensorManager::loop();
}