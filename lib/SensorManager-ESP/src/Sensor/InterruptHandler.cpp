
#include <Arduino.h>
#include <Sensor\InterruptHandler.h>


InterruptHandler::InterruptHandler(uint8_t id, uint8_t pinId)
{
    _id = id;
    _pinId = pinId;
}


InterruptHandler::~InterruptHandler()
{

}
