#ifndef __BinarySensor_h__
#define __BinarySensor_h__

//#include <Arduino.h>

#include <Web\TabbedAsyncDecl.h>
#include <Sensor\SensorProxy.h>

//class Device;

class BinarySensor : public SensorProxy
{
private:
    int _pinNumber;

public:
    
    BinarySensor(String _id, String _name, int pinNumber);
    ~BinarySensor();
    
    PageContentsCallback getCallback() override;

    static String onCallback(void *object);

};

#endif //__BinarySensor_h__
