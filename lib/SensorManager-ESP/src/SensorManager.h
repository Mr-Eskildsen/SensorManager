#ifndef __SENSORMANAGER_H__
#define __SENSORMANAGER_H__


	/**
		------ ------ ------ ------ ------ ------ library includes ------ ------ ------ ------ ------ ------
	*/

#include <SimpleArray.h>
#include <SimpleMap.h>
#include <Sensor\SensorProxy.h>
#include <Sensor\SensorEvent.h>


class Scheduler;
class InfoWebServer;
class SensorEvent;
	/**
		------ ------ ------ ------ ------ ------ macros for debugging ------ ------ ------ ------ ------ ------ 
	*/
#define INTR1_ID 0
#define INTR2_ID 1
#define INTR3_ID 2
#define INTR4_ID 3


	


	/**
		------ ------ ------ ------ ------ ------ STRUCTURES ------ ------ ------ ------ ------ ------

	*/

	/**
		------ ------ ------ ------ ------ ------ PROGMEM ------ ------ ------ ------ ------ ------
	*/
class SensorManager
{
private:    
	InfoWebServer *pWebSrv;
	SimpleMap<uint8_t, SensorProxy *> *pSensorsMap;
	SimpleMap<uint8_t, SensorProxy *> *pInterruptMap;
	SimpleArray<SensorProxy *> *pArrManualSensors;
	Scheduler *pScheduler;
	unsigned long lastUpdate;

	
//	int changed;
//	Task *pTask;

	
public:
	static void create(/*BoardMgrConfig* config*/); 

	static SensorManager* Instance();

private:    
	SensorManager();
	~SensorManager();
	static SensorManager *pInstance;

protected:
  	void _setup();
	void _loop();  

	void onTimer();
public:
  	static void setup();
	static void loop();  


protected:
	SensorProxy *getSensorByInterruptId(uint8_t intrId);
	void loadConfig();
	
	static void interruptHandler1();
	static void interruptHandler2();
	static void interruptHandler3();
	static void interruptHandler4();

	void _interrputHandler(uint8_t intrId);

	void onSensorChannelChanged(SensorEvent *pEvent);

};


#endif // __SENSORMANAGER_H__
