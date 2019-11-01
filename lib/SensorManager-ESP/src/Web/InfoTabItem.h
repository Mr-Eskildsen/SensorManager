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


/*
class InfoTabDropItem : public InfoTabItem 
{

private:
	//TODO:: MAKE DYNAMIC
	static const int ITEMS = 10;

	InfoTabItem* items[ITEMS] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
	int count = 0;
public:
	InfoTabDropItem(String id, String description, PageContentsCallback callback, void *object);
	

	void addDropItem(InfoTabItem* item) ;
	
	bool isDropItem() override {return true;}

	int GetDropItemCount() {return count;}

	InfoTabItem* GetDropItemAt(int idx) override
	{
		if ((idx<InfoTabDropItem::ITEMS) && idx>-1) 
		{
			return items[idx];
		}
		return NULL;
	};

};
*/
#endif // __InfoTabItem_h__
