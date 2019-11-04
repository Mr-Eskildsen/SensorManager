#ifndef __SENSOREVENT_H__
#define __SENSOREVENT_H__


enum SensorEventType {
    CHANGED = 0
};

enum SensorChannelType {
    ChannelBinary = 0,      /// Perform linear interpolation on the table
    ChannelTemperature = 1,  /// Perform parabolic interpolation on the table
    Channel_Humidity = 2
};

class SensorEvent
{
    String sensorId;
    String channelId;
    SensorChannelType sct;
    String value;

public:
    SensorEvent(String sensorId, String channelId, SensorChannelType sct, String value);
    ~SensorEvent();
    
    String getSensorId() {return sensorId;}
    String getChannelId() {return channelId;}
    SensorChannelType getChannelType () {return sct;}
    String getValue() {return value;}
};

#endif //__SENSOREVENT_H__ 
