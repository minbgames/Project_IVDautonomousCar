#include "FrameEvent.hpp"

bool FrameEvent::flag = false;
FrameEvent* FrameEvent::instance = nullptr;

FrameEvent* FrameEvent::getInstance()
{
	if(instance == nullptr) {
		instance = new FrameEvent();
		flag = true;
	}
	return instance;
}

