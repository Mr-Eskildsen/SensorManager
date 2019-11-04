#ifndef TabbedAsyncDecl_h
#define TabbedAsyncDecl_h
#include <Arduino.h>

#include <Sensor\SensorEvent.h>

//TODO:: MUST BE REMOVED
typedef std::function<String(void*)>			PageBuildCallback;


typedef std::function<String(void*)>			PageContentsCallback;


typedef std::function<void(SensorEvent* pEvent)>			SensorEventCallback;
#endif //TabbedAsyncDecl_h
