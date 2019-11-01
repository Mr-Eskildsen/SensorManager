#include <Arduino.h>

#include <Sensor\SensorProxy.h>

SensorProxy::SensorProxy(String _id, String _name)
{
     id =  _id; 
     name = _name;
}
    
SensorProxy::~SensorProxy()
{

}