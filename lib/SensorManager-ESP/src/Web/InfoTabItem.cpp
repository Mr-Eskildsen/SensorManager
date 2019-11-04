#include <Arduino.h>

#include <Web\InfoTabItem.h>
#include <Sensor\SensorProxy.h>
#include <Web\InfoTabItem.h>
	

InfoTabItem::InfoTabItem(String id, String description, PageContentsCallback callback, void *object)
{
	pVirtDevice = NULL;
	_id = id;
	_description = description;
	contentBuildCallback = callback;
	_object = object;
}

InfoTabItem::InfoTabItem(SensorProxy* pSensor)
{
	//TODO:: Do Something INIT STUFF here
	_id = pSensor->getSensorId();
	_description = pSensor->getName();
	contentBuildCallback = pSensor->getCallback();
	_object = pSensor;
	pVirtDevice = pSensor;
}

InfoTabItem::~InfoTabItem()
{
	if (pVirtDevice!=NULL)
		delete pVirtDevice;
}

	
String InfoTabItem::getContents() { 
	if (contentBuildCallback!=NULL) {
		return contentBuildCallback(_object); 
	}
	return "";
};
	
