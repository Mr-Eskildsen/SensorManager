
//#include <Arduino.h>
#include <Web\TabbedAsyncDecl.h>
#include <Sensor\BinarySensor.h>


BinarySensor::BinarySensor(String id, String name, int _pinNumber) 
     : SensorProxy(id, name)
{
     _pinNumber = _pinNumber;
};


BinarySensor::~BinarySensor()
{

};
    
PageContentsCallback BinarySensor::getCallback()
{
     return BinarySensor::onCallback;
}

String BinarySensor::onCallback(void *object) {
     return "<h4>Hello World</h4>";
}
