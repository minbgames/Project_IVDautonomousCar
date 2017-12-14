#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <time.h>
#include "../common/UDSServer.hpp"
#include "Vimba/VimbaCam.hpp"
#include "Vimba/FrameEvent.hpp"

#include "LaneDetector.hpp"

#define _CRT_SECURE_NO_WARNINGS

#ifdef _DEBUG
#pragma comment (lib, "opencv_world320d.lib")
#else
#pragma comment (lib, "opencv_world320.lib")
#endif // _DEBUG

using namespace cv;

// Declare matrices
Mat gpu_frame_output;
cuda::GpuMat gpu_frame_input;
// Vision indicator
vision::LaneDetector laneDetector;
// C.P measurement
char str[200];
UDSServer server("/tmp/gtserver");

// Time measurement
float begin, end;

int onFrameReceived(Mat frame);

int main()
{
	server.start();
	std::cout << "uds on" << std::endl;

	int key_pressed = 0;

	FrameEvent *fevent = FrameEvent::getInstance();
	fevent->callback = onFrameReceived;
	VimbaCam vimba;

	//VideoCapture capture("bin/tracking.mp4");

	//if (!capture.isOpened()) {
	//	perror("Invalid video files");
	//	return -1;
	//}

	vimba.start();
	// Calculating Size of frame
	//double dWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);
	//double dHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);

	double dWidth = 1280;
	double dHeight = 960;

	std::cout << "Frame Size = " << dWidth << "x" << dHeight << std::endl;

	//Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));
	namedWindow("Processed Video", 0);
	resizeWindow("Processed Video",640,480);

	// Record
	//VideoWriter writer;
	//writer.open("Hi1.avi", writer.fourcc('M', 'J', 'P', 'G'), 25, cv::Size(1280, 720));


	// Data printing
	//std::ofstream out;
	//out.open("result.txt");
	int lock;
	std::cin >> lock;


	server.close();
	//out.close();
}


int onFrameReceived(Mat frame)
{
	//capture >> frame;		//���� ����

	gpu_frame_input.upload(frame);
	begin = clock();
	auto local = frame.clone();
	auto roadModel = laneDetector.DetectLane(local);
	auto center = laneDetector.FindingCenter();
	auto curvature = laneDetector.FindingCurvature();
	end = clock();
	std::cout << "Elapsed time : " << ((end - begin) / 1000) << "ms" << std::endl;
	sprintf(str, "Gap from center : %.3f", 640.0f - center);
	putText(local, str, Point2f(40, 60), FONT_HERSHEY_TRIPLEX, 1, Scalar(0, 255, 0));
	sprintf(str, "Curvature : %.3f", curvature);
	putText(local, str, Point2f(40, 100), FONT_HERSHEY_TRIPLEX, 1, Scalar(0, 255, 0));
	//out << float(640 - center);

	//out << " ";

	server.sendFloat(640.0f - center);

	//writer.write(local);
	imshow("Processed Video", local);

	//waitKey(25);
	return 0;

}
