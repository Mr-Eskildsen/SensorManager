#ifndef __INTERRUPTHANDLER_H__
#define __INTERRUPTHANDLER_H__

class InterruptHandler {

private:
    uint8_t _id;
    uint8_t _pinId;
public:
    InterruptHandler(uint8_t id, uint8_t pinId);
    ~InterruptHandler();

    uint8_t getId()    {return _id;};

};
#endif //__INTERRUPTHANDLER_H__
