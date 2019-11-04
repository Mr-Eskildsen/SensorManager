
#ifndef __SENSORPROXY_H__
#define __SENSORPROXY_H__

#include <Arduino.h>

#include <Web\TabbedAsyncDecl.h>

class SensorProxy
{
    String _id;
    String _name;

    unsigned long lastRead;
protected:
    SensorEventCallback callback;
public:
    SensorProxy(String id, String name);
    virtual ~SensorProxy() = 0;

    String getSensorId() {return _id;}
    virtual uint32_t getUniqueId() = 0;
    
    String getName() {return _name;}
    
    void registerEventCallback(SensorEventCallback _callback);


    virtual bool useInterrupt() {return false;};
    virtual int getInterruptId() {return -1;};
    
    virtual void onInterrupt(uint8_t intrId) = 0;

    virtual PageContentsCallback getCallback() = 0;

    bool read(unsigned long curTime = 0, bool force = false);
    virtual String getValue(int channelId = 0) = 0;    
    
protected:
    virtual bool onRead(bool force) = 0;    
    virtual uint32_t getReadInterval() = 0;
};

#endif //__SENSORPROXY_H__
