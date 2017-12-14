#include <iostream>
#include <cstdio>
#include <thread>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <time.h>
#include "Vimba/VimbaCam.hpp"
#include "Vimba/FrameEvent.hpp"
#include "../common/UDSServer.hpp"

#include "LaneDetector.hpp"

#define _CRT_SECURE_NO_WARNINGS

#ifdef _DEBUG
#pragma comment (lib, "opencv_world320d.lib")
#else
#pragma comment (lib, "opencv_world320.lib")
#endif // _DEBUG

using namespace cv;
using namespace std;
namespace chro = std::chrono;
namespace tthr = std::this_thread;

UDSServer server("/tmp/gtserver");

// Declare matrices
Mat frame, gpu_frame_output;
cuda::GpuMat gpu_frame_input;
// Vision indicator
vision::LaneDetector laneDetector;

// C.P measurement
char str[200];

// Time measurement
int begin_time, end_time;

// Data printing
std::ofstream out;
VideoWriter writer;

int onFrameReceived(Mat frame);


int handler2()
{
	int cmd;
	int value;
	while(1) {
		cmd = server.recvInt();
		value = server.recvInt();
		if(cmd == 0)
			laneDetector.SetSteering(value);
		else if(cmd == 1) {
			laneDetector.Inputsign(value);
		}
	}
	return 0;
}

/***************************************/
int k = 2;
int max_goodmatch;
int final_goodmatch;
int roi_ok=0;
int traffic_ok=0;
int flag = 0;
int roi_off_ok=0;
float rec_ratio=0;
int flag2=1;
int result_traffic=0;

int output_traffic[8]={-1,0,1,2,3,4,5,6};
//setting
Mat frame_origin;
Mat frame_gray;
Mat resized_frame_gray;
Mat hsv_frame;
Mat binary_frame_red;
Mat binary_frame_red1;
Mat binary_frame_red2;
Mat binary_frame_blue;
Mat binary_frame_merge;
Mat morphological_frame;
Mat resized_frame;
Mat wanted_frame;
Mat draw_frame;

// red value range
Scalar lowerb_red1(0, 140, 0);
Scalar upperb_red1(15, 255, 255);
Scalar lowerb_red2(175, 140, 0);
Scalar upperb_red2(179, 255, 255);
// blue value range
Scalar lowerb_blue(100, 215, 0);
Scalar upperb_blue(130, 255, 255);

//0.횡단보도 1.협로구간 2.동적장애물 3.정적장애물 4.곡선코스 5.U턴 6.자동주차
Mat img[7];


int empty_test=0;
/***************************************/





int main()
{

	img[0] = imread("source/img_0.JPG", IMREAD_GRAYSCALE);
	img[1] = imread("source/img_1.JPG", IMREAD_GRAYSCALE);
	img[2] = imread("source/img_2.JPG", IMREAD_GRAYSCALE);
	img[3] = imread("source/img_3.JPG", IMREAD_GRAYSCALE);
	img[4] = imread("source/img_4.JPG", IMREAD_GRAYSCALE);
	img[5] = imread("source/img_5.JPG", IMREAD_GRAYSCALE);
	img[6] = imread("source/img_6.JPG", IMREAD_GRAYSCALE);
	for (int n = 0; n < 7; n++) { empty_test = empty_test || img[n].empty(); }
	if (empty_test) {
		printf("Cannot open sign imgs!");
		return -1;
	}








	server.start();

	//std::thread th1(handler1);
	std::thread th2(handler2);
	std::cout << "uds on" << std::endl;

	int key_pressed = 0;

	FrameEvent *fevent = FrameEvent::getInstance();
	fevent->callback = onFrameReceived;
	//VimbaCam vimba;
	//vimba.start();

	VideoCapture capture(0);

	//if (!capture.isOpened()) {
	//	perror("Invalid video files");
	//	return -1;
	//}

	// Calculating Size of frame
	//double dWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);
	//double dHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
	double dWidth = 1280.0;
	double dHeight = 960.0;

	std::cout << "Frame Size = " << dWidth << "x" << dHeight << std::endl;

	Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));
	// namedWindow("Processed Video", 0);
	// resizeWindow("Processed Video",640,480);


	// Record
	writer.open("Hi1.avi", writer.fourcc('M', 'J', 'P', 'G'), 18, cv::Size(1280,960 ));

	//out.open("result.txt");


	/*
	VideoCapture vc0(0);
	if(!vc0.isOpened()) {
		printf("cannot open vc0!\n");
		return 1;
	}
	VideoCapture vc1(1);
	if(!vc1.isOpened()) {
		printf("cannot open vc1!\n");
		return 1;
	}

	Mat img0;
	Mat img1;

	namedWindow("cam0", WINDOW_NORMAL);
	namedWindow("cam1", WINDOW_NORMAL);
	resizeWindow("cam0", 400, 400);
	resizeWindow("cam1", 400, 400);

	while(1) {
		vc0 >> img0;
		vc1 >> img1;
		if(img0.empty()) continue;
		if(img1.empty()) continue;
		moveWindow("cam1", 0, 350);
		imshow("cam0", img0);
		imshow("cam1", img1);
		if(cvWaitKey(30) == 'q')
			break;
	}
	*/

	while(1) {
		capture >> frame;		//���� ����
		//cvtColor(frame2, frame, CV_BGR2RGB);
		//int nnn;
		//std::cin >> nnn;
		gpu_frame_input.upload(frame);
		begin_time = clock();
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
			auto centre = laneDetector.FindingCentre();
			auto curvature = laneDetector.FindingCurvature();
			auto distance = laneDetector.stop_line_distance();
			auto goal = laneDetector.Parkgoal();
			end_time = clock();
			//std::cout << "수행시간 : " << ((end - begin) / 1000) << "초" << std::endl;
			float t = 1 / ((end_time - begin_time) / 1000);

			sprintf(str, "Gap from center : %.3f", 640.0f - center);
			putText(local, str, Point2f(40, 60), FONT_HERSHEY_TRIPLEX, 1, Scalar(0, 255, 0));
			sprintf(str, "Gap from centre : %.3f", 640.0f - centre);
			putText(local, str, Point2f(40, 100), FONT_HERSHEY_TRIPLEX, 1, Scalar(0, 255, 0));
			sprintf(str, "FPS : %.2f", 1000 * t);
			putText(local, str, Point2f(40, 140), FONT_HERSHEY_TRIPLEX, 1, Scalar(0, 255, 0));
			sprintf(str, "Curvature : %.3f", curvature);
			putText(local, str, Point2f(40, 180), FONT_HERSHEY_TRIPLEX, 1, Scalar(0, 255, 0));
			//out << float(640 - center);		// test renew
			//while (key_pressed != 32)
				//key_pressed = waitKey(0);
				server.sendFloat(640.0f - center);
				server.sendFloat(distance);
				server.sendFloat(goal);
		}


		writer.write(frame);
		//namedWindow("Processed Video", 0);
		//resizeWindow("Processed Video",640,480);
		//imshow("Processed Video", local);
	}
	//int lock;
	//std::cin >> lock;

	//th1.join();
	th2.join();
	server.close();
	//vimba.stopAcquisition();
	server.close();
	//out.close();

	return 0;
}


int onFrameReceived(Mat frame)
{






	try {
		frame_origin = frame.clone();
		//frame >> frame_origin; // get a new frame from webcam
		resize( frame_origin, frame_origin, Size( 640, 480 ), 0, 0, CV_INTER_CUBIC );
	}
	catch (Exception& e) {
		std::cerr << "Exception occurred. Ignoring frame... " << e.err
			<< std::endl;
	}

	cvtColor(frame_origin, hsv_frame, COLOR_BGR2HSV);
	cvtColor(frame_origin, frame_gray, COLOR_BGR2GRAY);

	inRange(hsv_frame, lowerb_red1, upperb_red1, binary_frame_red1);
	inRange(hsv_frame, lowerb_red2, upperb_red2, binary_frame_red2);
	inRange(hsv_frame, lowerb_blue, upperb_blue, binary_frame_blue);

	binary_frame_red = binary_frame_red1 | binary_frame_red2;
	binary_frame_merge = binary_frame_red | binary_frame_blue;

	//morphological opening 작은 점들을 제거
	erode(binary_frame_merge, morphological_frame, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );
	dilate( morphological_frame, morphological_frame, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );

	//morphological closing 영역의 구멍 메우기
	dilate( morphological_frame, morphological_frame, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );
	erode(morphological_frame, morphological_frame, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );

	//라벨링
	Mat img_labels,stats, centroids;
	int numOfLables = connectedComponentsWithStats(morphological_frame, img_labels, stats, centroids, 8,CV_32S);

	//영역박스 그리기
	int max = -1, idx=0;
	for (int j = 1; j < numOfLables; j++) {
			int area = stats.at<int>(j, CC_STAT_AREA);
			if ( max < area )
			{
					max = area;
					idx = j;
			}
	}

	int left = stats.at<int>(idx, CC_STAT_LEFT);
	int top  = stats.at<int>(idx, CC_STAT_TOP);
	int width = stats.at<int>(idx, CC_STAT_WIDTH);
	int height  = stats.at<int>(idx, CC_STAT_HEIGHT);

	//rectangle( frame, Point(left,top), Point(left+width,top+height), Scalar(0,0,255),1 );

	//rectangle( frame, Point(left,top), Point(left+width,top+height), Scalar(0,0,255),1 );

		rec_ratio =(float)width/height;

		if(rec_ratio>1.2||rec_ratio<0.8){
			width=1;
			height=1;
		}
		else{
			if(width >100) width = 1;
			if(height >100)	height = 1;
		}

		max_goodmatch = 0;
		final_goodmatch = -1;

		if(width>50 && height>50){
			Rect rect(Point(left,top), Point(left+width,top+height));

			wanted_frame = frame(rect);

			resize( wanted_frame, resized_frame, Size( 190, 190 ), 0, 0, CV_INTER_CUBIC );
			cvtColor(resized_frame, resized_frame_gray, CV_BGR2GRAY);

			roi_ok=roi_ok+1;
			if(roi_ok>40){
				if(flag2){
					traffic_ok=traffic_ok+1;
					if(traffic_ok>7) traffic_ok=7;
					flag2=0;
					result_traffic= traffic_ok;
				}
				flag=1;
			}
			else{
				flag2=1;
			}
			// imshow("output_frame",resized_frame);
		}
		else{
			roi_ok=0;
			if(flag){
				roi_off_ok=roi_off_ok+1;
				if(roi_off_ok>50){
					result_traffic=0;
					flag=0;
					roi_off_ok=0;
				}
			}
		}

		// imshow("original",frame);
		// imshow("morphological_frame",morphological_frame);
		// final_goodmatch //roi_ok 출력

		cout << "width-height: " << width << " , "<< height << endl;
		cout << "roi: " << roi_ok << "   tra: " << traffic_ok <<"   flag: " << flag <<  "roff: " << roi_off_ok << " rec_ratio:" << rec_ratio << endl;
		cout << "flag2: " << flag2 << " result_traffic: " << result_traffic << endl;
		cout << "output: " << output_traffic[result_traffic] << " traffic_ok:" << traffic_ok << endl;


	return 0;
}
