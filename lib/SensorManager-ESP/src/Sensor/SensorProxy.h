
#ifndef __SensorProxy_h__
#define __SensorProxy_h__

#include <Arduino.h>

#include <Web\TabbedAsyncDecl.h>

class SensorProxy
{
    String id;
    String name;
public:
    SensorProxy(String _id, String _name);
    virtual ~SensorProxy() = 0;

    String getId() {return id;}
    String getName() {return name;}

    virtual PageContentsCallback getCallback() = 0;

};

#endif //__SensorProxy_h__
