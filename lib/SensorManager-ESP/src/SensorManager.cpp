#ifdef ESP8266 || ESP32
#define ISR_PREFIX ICACHE_RAM_ATTR
#else
#define ISR_PREFIX
#endif


#define _TASK_STD_FUNCTION


#include <Arduino.h>

#include <TaskScheduler.h>

#include <Web\InfoWebServer.h>
#include <Sensor\BinarySensor.h>
#include <SimpleMap.h>
#include <SensorManager.h>


SensorManager *SensorManager::pInstance = 0;




void SensorManager::create(/*BoardMgrConfig* config*/) 
{
    SensorManager::pInstance = new SensorManager();
};

SensorManager* SensorManager::Instance()
{
    if (SensorManager::pInstance==NULL) {
        return NULL;
    }
    return SensorManager::pInstance;
};



SensorManager::SensorManager()
{
    pWebSrv = NULL;
    pSensorsMap = NULL;
    pInterruptMap = NULL;

    pScheduler = NULL;
	
    lastUpdate = 0;
    //changed = 0;
}

SensorManager::~SensorManager()
{
    if (pWebSrv) {
        delete pWebSrv;
    }

 	if (pScheduler) {
		delete pScheduler;
	}
       //TODO:: Delete all sensors and delete maps
}


void SensorManager::onSensorChannelChanged(SensorEvent* pEvent)
{
    Serial.print("Received event from '");
    Serial.print(pEvent->getSensorId());
    Serial.print(".");
    Serial.print(pEvent->getChannelId());
    Serial.print(" Value=");
    //Serial.print(pEvent->getChannelType());
    Serial.println(pEvent->getValue());

    if (pWebSrv)
        pWebSrv->onSensorEvent(pEvent);
}


ISR_PREFIX void SensorManager::interruptHandler1()
{
    //Serial.println("Hello World -> SensorManager::interruptHandler1()");
    Instance()->_interrputHandler(INTR1_ID);
}

ISR_PREFIX void SensorManager::interruptHandler2()
{
    Serial.println("Hello World -> SensorManager::interruptHandler2()");
    Instance()->_interrputHandler(INTR2_ID);
}

ISR_PREFIX void SensorManager::interruptHandler3()
{
    Serial.println("Hello World -> SensorManager::interruptHandler3()");
    Instance()->_interrputHandler(INTR3_ID);
}

ISR_PREFIX void SensorManager::interruptHandler4()
{
    Serial.println("Hello World -> SensorManager::interruptHandler4()");
    Instance()->_interrputHandler(INTR4_ID);
}

void SensorManager::_interrputHandler(uint8_t intrId)
{
    SensorProxy* pSensor = getSensorByInterruptId(intrId);

    //Force a sensor read
    pSensor->read(millis(), true);
    /*
    uint8_t pin = D5;
        //HEST
   int newValue = digitalRead(pin);
    
    Serial.print("[INTERRUPT (");
    Serial.print(intrId);
    Serial.print(")] Correct Generic handler. New value for pin='");
    Serial.print(pin);
    Serial.print("' => ");
    Serial.println(newValue);
*/
}

SensorProxy *SensorManager::getSensorByInterruptId(uint8_t intrId)
{
    return pInterruptMap->getData(intrId);
}

/*
void SensorManager::registerThing(SensorProxy* thing) 
{
    if (nextSensorIdx<MAX_SENSORS) {
        Serial.print("Registering sensor '");
        Serial.print(nextSensorIdx);
        Serial.print("'");

        arrSensors[nextSensorIdx] = thing;
        
        //TODO:: More generic -> 'INPUT' and 'CHANGE' from class
        
        pinMode(thing->getPinId(), INPUT);
        if (thing->HasInterrupt() && (nextIntrIdx<MAX_INTERRUPTS)) {
            
            int interruptId = digitalPinToInterrupt(thing->getPinId());
            thing->SetInterruptId(interruptId);
            switch(nextIntrIdx) {
            case 0:
                attachInterrupt(interruptId , BoardManager::interruptHandler1, CHANGE);
                break;
            case 1:
                attachInterrupt(interruptId , BoardManager::interruptHandler2, CHANGE);
                break;
            case 2:
                attachInterrupt(interruptId , BoardManager::interruptHandler3, CHANGE);
                break;
            case 3:
                attachInterrupt(interruptId , BoardManager::interruptHandler4, CHANGE);
                break;

            }
            arrInterrupts[nextIntrIdx] = thing;

            Serial.print(" and register interrupt '");
            Serial.print(nextIntrIdx);
            Serial.print("'");
            nextIntrIdx++;
        }
        nextSensorIdx++;
        Serial.println(" - DONE!");
    }
    
    
};
*/


void SensorManager::setup()
{
    if (Instance()!=NULL) {
        Instance()->_setup();
    }
}


void SensorManager::loop()
{
    if (Instance()!=NULL) {
        Instance()->_loop();
    }
}




void SensorManager::_setup()
{
 	pSensorsMap = new SimpleMap<uint8_t, SensorProxy *>([](uint8_t &a, uint8_t &b) -> int {
								if (a == b) return 0;      // a and b are equal
								else if (a > b) return 1;  // a is bigger than b
								else return -1;            // a is smaller than b
							});

    pArrManualSensors = new SimpleArray<SensorProxy *>();

    pInterruptMap = new SimpleMap<uint8_t, SensorProxy *>([](uint8_t &a, uint8_t &b) -> int {
								if (a == b) return 0;      // a and b are equal
								else if (a > b) return 1;  // a is bigger than b
								else return -1;            // a is smaller than b
							});


    if (pWebSrv)
    {
        delete pWebSrv;
        pWebSrv = NULL;
    }



    loadConfig();
    

    pWebSrv = new InfoWebServer(80);
    if (pWebSrv) {
        pWebSrv->setup();
        pWebSrv->begin();
    }
    
    pScheduler = new Scheduler();
	
	pScheduler->init();
	Serial.println("[SensorManager] - Adding timer callback");
	std::function<void()> onTimerCallback = std::bind(&SensorManager::onTimer, this);
	Task *pTask =  new Task(100, TASK_FOREVER, onTimerCallback);
	pScheduler->addTask(*pTask);



	if (pScheduler) {
		Serial.println("[SensorManager] Enable all tasks");
		pScheduler->enableAll();
	}

//    attachInterrupt(digitalPinToInterrupt(D5), SensorManager::interruptHandler1, CHANGE);

}

void SensorManager::_loop()
{
        
	if (pScheduler) {
		pScheduler->execute();
	}

    
    if (pWebSrv) {
        pWebSrv->loop();
    }

    
}


/* ************************************
**
**
**
************************************ */

void SensorManager::loadConfig()
{
    //TODO:: Read from JSON Config
    //BinarySensor *arrSensors[2];
    SensorProxy *arrSensors[2];
    int sensorCount = 2;

    arrSensors[0] = new BinarySensor("pir1", "PIR Sensor 1", D5, true);
	arrSensors[1] = new BinarySensor("pir2", "PIR Sensor 2", D6);


    for (int idx=0;idx<sensorCount;idx++) {
        SensorProxy *pCurSensor = arrSensors[idx];
        
        pSensorsMap->put(pCurSensor->getUniqueId(), pCurSensor);

        Serial.print("Registering sensor Id='");
        Serial.print(pCurSensor->getSensorId());
        Serial.print("', pinId='");
        Serial.print(pCurSensor->getUniqueId());
        
        


        if (pCurSensor->useInterrupt()) {
                
    
            uint8_t interruptId = pCurSensor->getInterruptId(); 
            Serial.print("', IntrId='");        
            Serial.print(interruptId);        
            
            pInterruptMap->put(interruptId, pCurSensor);
            switch (pInterruptMap->size()) {
            case 1:
                //Serial.print("' (Static handler1)");        
                attachInterrupt(interruptId, SensorManager::interruptHandler1, CHANGE);
                break;
            case 2:
                // Serial.print("' (Static handler2)");        
                attachInterrupt(interruptId, SensorManager::interruptHandler2, CHANGE);
                break;
            case 3:
               // Serial.print("' (Static handler3)");        
                attachInterrupt(interruptId, SensorManager::interruptHandler3, CHANGE);
                break;
            case 4:
                //Serial.print("' (Static handler4)");        
                attachInterrupt(interruptId, SensorManager::interruptHandler4, CHANGE);
                break;
            }
    
        }
        else {
            Serial.print("' (Manual Read)");        
            pArrManualSensors->add(pCurSensor);
        }
        Serial.println("'");

        pCurSensor->registerEventCallback(std::bind(&SensorManager::onSensorChannelChanged, this, std::placeholders::_1));          

        //pSensorsMap->put(D6, pSensor2Config);
        //pInterruptMap->put(interruptId, pSensor1Config)
    }
    
    //pSensor1Config
    //attachInterrupt(interruptId, SensorManager::interruptHandler1, CHANGE);
}



void SensorManager::onTimer()
{
	unsigned long curMillis = millis();
	//if(((lastUpdate+1000)<curMillis) && globalClient != NULL && globalClient->status() == WS_CONNECTED){
      String randomNumber = String(random(0,20));
      //globalClient->text(randomNumber);
	
        
    
        for (int idx=0;idx<pArrManualSensors->size();idx++) 
        {   
            SensorProxy *pSensor = pArrManualSensors->get(idx);
            if (pSensor->read(curMillis)) {
//                changed++;
/*
                Serial.print("Reading sensor '");
                Serial.print(pSensor->getName());
                Serial.print("' changed (Value='");
                Serial.print(pSensor->getValue());
                Serial.println("')");
*/
//                changed = 0;
                lastUpdate = curMillis;
            }

/*		for(int idx=0; idx<mapConnections->size();idx++) {
			AsyncWebSocketClient* conn = mapConnections->getData(idx);
			if (conn != NULL && conn->status() == WS_CONNECTED){
				//TODO:: Fix this
				int val = digitalRead(D5);
				if (val==0) {
					conn->text(String("Untriggered"));
				}
				else {
					conn->text(String("Triggered"));
				}
				//conn->text(randomNumber);
			}
		}
        */

		
   }

}