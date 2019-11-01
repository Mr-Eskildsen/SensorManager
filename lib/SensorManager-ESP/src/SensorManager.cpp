#include <Arduino.h>
#include <Web\InfoWebServer.h>

#include <SensorManager.h>


SensorManager::SensorManager()
{
    pWebSrv = NULL;
}

SensorManager::~SensorManager()
{
    if (pWebSrv) {
        delete pWebSrv;
    }
}

void SensorManager::setup()
{
    pWebSrv = new InfoWebServer(80);
    if (pWebSrv) {
        pWebSrv->setup();
        pWebSrv->begin();
    }

}

void SensorManager::loop()
{
    if (pWebSrv) {
        pWebSrv->loop();
    }
}
