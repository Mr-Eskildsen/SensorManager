
//#include <Arduino.h>
#include <Web\TabbedAsyncDecl.h>
#include <Sensor\SensorEvent.h>
#include <Sensor\BinarySensor.h>



#define HARDCODED_INTERVAL    100

BinarySensor::BinarySensor(String id, String name, uint8_t pinNumber, bool useInterrupt) 
     : SensorProxy(id, name)
{
     _pinNumber = pinNumber;
     _useInterrupt = useInterrupt;

     curValue = -1; 
};


BinarySensor::~BinarySensor()
{

};


void BinarySensor::onInterrupt(uint8_t intrId)
{

}

bool BinarySensor::onRead(bool force) {
     int prevValue = curValue;
     curValue = digitalRead(_pinNumber);
     
     if ((curValue!=prevValue) || force)  {
          SensorEvent se = SensorEvent(getSensorId(), "state", ChannelBinary, String(curValue) );
          if (callback!=NULL) {
               callback(&se);
          }
     }

     return (curValue!=prevValue) ? true : false;
}


uint32_t  BinarySensor::getReadInterval()
{
     //TODO Dont HARDCODE!
     return HARDCODED_INTERVAL;
}

PageContentsCallback BinarySensor::getCallback()
{
     return BinarySensor::onCallback;
}

String BinarySensor::onCallback(void *object) {
     return "<h4>Hello World</h4>";
}

String  BinarySensor::getValue(int channelId)
{
     return String(curValue);
}