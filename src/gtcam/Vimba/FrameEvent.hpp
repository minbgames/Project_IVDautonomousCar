#pragma once

#include <opencv2/core/core.hpp>
#include <functional>

class FrameEvent {
private:
	FrameEvent() {};
	static bool flag;
	static FrameEvent* instance;

public:
	static FrameEvent* getInstance();
	virtual ~FrameEvent()
	{
		flag = false;
	}
	std::function<int(cv::Mat)> callback = nullptr;
};

