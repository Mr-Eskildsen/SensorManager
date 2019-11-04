#ifndef __BinarySensor_h__
#define __BinarySensor_h__

//#include <Arduino.h>

#include <Web\TabbedAsyncDecl.h>
#include <Sensor\SensorProxy.h>

//class Device;

class BinarySensor : public SensorProxy
{
private:
    uint8_t _pinNumber;
    uint8_t _useInterrupt;
    int curValue;
public:
    
    BinarySensor(String _id, String _name, uint8_t pinNumber, bool useInterrupt = false);
    ~BinarySensor();
    
    uint32_t getUniqueId() override {return getPinNumber();};
    uint32_t getPinNumber()  {return _pinNumber;}
    
    bool useInterrupt() override {return _useInterrupt;};
    int getInterruptId() override {return digitalPinToInterrupt(getPinNumber());};
    void onInterrupt(uint8_t intrId) override;
  
    //bool read() override;
    String getValue(int channelId = 0);

    PageContentsCallback getCallback() override;

    static String onCallback(void *object);

protected:
    bool onRead(bool force) override;    
    uint32_t getReadInterval() override;

};

#endif //__BinarySensor_h__
