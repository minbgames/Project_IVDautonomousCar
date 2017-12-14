#pragma once

#include <thread>

class Receiver
{

protected:
    void (*exFunc)(void);
    std::thread *recvThread;
    bool threadFlag;
    virtual void startThread() = 0;
    virtual void inFunc() = 0;

public:
    Receiver();
    virtual ~Receiver();
    void setHandler(void (*pf)(void));
    void stopThread();
};
