#include <iostream>
#include <thread>

#include "Receiver.hpp"

Receiver::Receiver()
{
    std::cout << "base constructor called" << std::endl;
}

Receiver::~Receiver()
{
}

void Receiver::setHandler(void (*pf)(void))
{
    exFunc = pf;
}

void Receiver::stopThread()
{
    threadFlag = false;
}
