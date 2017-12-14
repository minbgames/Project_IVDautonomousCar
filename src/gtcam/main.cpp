#include <iostream>
#include <cstdio>
#include <thread>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <time.h>
#include "../common/UDSServer.hpp"

#include "LaneDetector.hpp"


#define _CRT_SECURE_NO_WARNINGS

#ifdef _DEBUG
#pragma comment (lib, "opencv_world320d.lib")
#else
#pragma comment (lib, "opencv_world320.lib")
#endif // _DEBUG


using namespace cv;
namespace chro = std::chrono;
namespace tthr = std::this_thread;

// Vision indicator
vision::LaneDetector laneDetector;

int handler1()
{
	int input;
	while(1) {
		std::cout << "input : ";
		std::cin >> input;
		laneDetector.Inputsign(input);
	}
	return 0;
}


int main()
{

	UDSServer server("/tmp/gtserver");
	server.start();
	std::thread th1(handler1);

	std::cout << "uds on" << std::endl;

	int key_pressed = 0;

	VideoCapture capture(0);
	//VideoCapture capture("bin/Hi1.avi");

	if (!capture.isOpened()) {
		perror("Invalid video files");
		return -1;
	}


	// Calculating Size of frame
	double dWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);
	double dHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);

	std::cout << "Frame Size = " << dWidth << "x" << dHeight << std::endl;

	Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));
	namedWindow("Processed Video", 0);
	resizeWindow("Processed Video",640,480);
	// Declare matrices
	Mat frame, gpu_frame_output;
	cuda::GpuMat gpu_frame_input;


	// Record
	//VideoWriter writer;
	//writer.open("Hi1.avi", writer.fourcc('M', 'J', 'P', 'G'), 25, cv::Size(1280, 720));

	// C.P measurement
	char str[200];

	// Time measurement
	float begin, end;

	// Data printing
	std::ofstream out;
	//out.open("result.txt");



	while (key_pressed != 27) {
		capture >> frame;		//���� ����

		//cvtColor(frame2, frame, CV_BGR2RGB);
		//int nnn;
		//std::cin >> nnn;
		gpu_frame_input.upload(frame);
		begin = clock();
		auto iserror = 0;
		auto local = frame.clone();
		try {
			auto roadModel = laneDetector.DetectLane(local);
		}
		catch (int exception) {
			std::cout << "exception : " << exception << std::endl;
			iserror = 1;
		}
		if (iserror == 0) {

			auto center = laneDetector.FindingCenter();
			auto curvature = laneDetector.FindingCurvature();
			auto distance = laneDetector.stop_line_distance();
			auto goal = laneDetector.Parkgoal();

			end = clock();
			//std::cout << distance << std::endl;
			float t = 1 / ((end - begin) / 1000);

			sprintf(str, "Gap from center : %.3f", 320.0f - center);
			putText(local, str, Point2f(40, 60), FONT_HERSHEY_TRIPLEX, 1, Scalar(0, 255, 0));
			sprintf(str, "FPS : %.2f", 1000 * t);
			putText(local, str, Point2f(40, 140), FONT_HERSHEY_TRIPLEX, 1, Scalar(0, 255, 0));
			sprintf(str, "Curvature : %.3f", curvature);
			putText(local, str, Point2f(40, 180), FONT_HERSHEY_TRIPLEX, 1, Scalar(0, 255, 0));
			server.sendFloat(320.0f - center);
			std::cout << 320.0f - center << std::endl;
			//out << float(640 - center);		// test renew
			//while (key_pressed != 32)
				//key_pressed = waitKey(0);
		}


		//writer.write(local);

		imshow("Processed Video", local);

		key_pressed = waitKey(10);
	}

	//th1.join();

	server.close();
	//out.close();
}
