#ifndef __SENSORMANAGER_H__
#define __SENSORMANAGER_H__


	/**
		------ ------ ------ ------ ------ ------ library includes ------ ------ ------ ------ ------ ------
	*/

class InfoWebServer;

	/**
		------ ------ ------ ------ ------ ------ macros for debugging ------ ------ ------ ------ ------ ------ 
	*/

	


	/**
		------ ------ ------ ------ ------ ------ STRUCTURES ------ ------ ------ ------ ------ ------

	*/

	/**
		------ ------ ------ ------ ------ ------ PROGMEM ------ ------ ------ ------ ------ ------
	*/
class SensorManager
{

	InfoWebServer *pWebSrv;

public:    
	SensorManager();
	~SensorManager();

  	void setup();
	void loop();  


};


#endif // __SENSORMANAGER_H__
