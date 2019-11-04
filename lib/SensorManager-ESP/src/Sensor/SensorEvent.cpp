#include <Arduino.h>

#include <Sensor\SensorEvent.h>


SensorEvent::SensorEvent(String _sensorId, String _channelId, SensorChannelType _sct, String _value)
{
    sensorId = _sensorId;
    channelId = _channelId;
    sct = _sct;
    value = _value;
}

SensorEvent::~SensorEvent()
{

}