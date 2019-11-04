#include <Arduino.h>

#include <Sensor\SensorProxy.h>

SensorProxy::SensorProxy(String id, String name)
{
     lastRead = 0;

     _id =  id; 
     _name = name;
     callback = NULL;

}
    
SensorProxy::~SensorProxy()
{

}
void SensorProxy::registerEventCallback(SensorEventCallback _callback)
{
     
     Serial.println("[SensorProxy] Registering event callback");
     callback = _callback;
}

bool SensorProxy::read(unsigned long curTime, bool force)
{

     if (useInterrupt() && !force) {
          //Not read => No change
          return false;
     }
     if (curTime==0)
          curTime = millis();

     if ((lastRead + getReadInterval() > curTime) && (force==false))
          return false;
/*          
     Serial.print("LastRead=");
     Serial.print(lastRead);
     Serial.print(", CurTime=");
     Serial.print(curTime);
     Serial.print(", (LastRead+Interval)=");
     Serial.println(lastRead + getReadInterval());
     */
     lastRead = curTime;
     return onRead(force);
}

