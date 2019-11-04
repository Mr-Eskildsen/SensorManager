#ifndef __InfoTabItem_h__
#define __InfoTabItem_h__

#include <Arduino.h>

//#include "IOTAppStory.h"
//#include <ESPAsyncWebServer.h>               // https://github.com/me-no-dev/ESPAsyncWebServer
#include <Web\TabbedAsyncDecl.h>
#include <Sensor\SensorProxy.h>

class SensorProxy;

class InfoTabItem
{
private:
	String _id;
	String _description;
	void *_object;
	SensorProxy* pVirtDevice;
	PageBuildCallback contentBuildCallback = NULL;
public:
	/*, PageBuildCallback callback, void *object*/
	InfoTabItem(String id, String description, PageBuildCallback callback, void *object);
	InfoTabItem(SensorProxy* pSensor);
	~InfoTabItem();

	
	String getId() { return _id; };
	String getDescription() { return _description; };
	
	String getContents();
	
	virtual bool isDropItem() {return false;};
	virtual int GetDropItemCount() {return 0;};
	virtual InfoTabItem* GetDropItemAt(int idx) {return NULL;};
};


#endif // __InfoTabItem_h__
