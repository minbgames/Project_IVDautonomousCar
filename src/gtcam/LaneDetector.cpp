#include <algorithm>
#include <iterator>
#include <math.h>
#include "LaneDetector.hpp"
#include "LaneModels.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/cudafilters.hpp>
#include "PointsToLanesConverter.hpp"
#include "LaneMarkDetector.hpp"
#include <iostream>

#define PI 3.141592

int inputsign;
double center;
double centre;
double curvature;
double distance;
double goal;
std::vector<cv::Point2f> vehicle_lane;
std::vector<cv::Point2f> parking_lane;

namespace vision
{

LaneDetector::LaneDetector()
{

}

void Mymouse(int event, int x, int y, int flag, void* param)
{
	int startX, startY;
	switch (event) {
		case CV_EVENT_LBUTTONDOWN:
		std::cout << "Left button down X= " << x/2 << ", Y= " << y/2 << std::endl;
		startX = x;
		startY = y;
		break;
	}
}




cv::Mat LaneDetector::FindPixelsThatMayBelongToLane(const cv::cuda::GpuMat& input)
{

	cv::cuda::GpuMat road(input.clone());
	cv::cuda::GpuMat grayscale_gpu;
	cv::cuda::cvtColor(road, grayscale_gpu, CV_BGR2GRAY);	// gray scale
	cv::cuda::GpuMat hls_gpu;
	cv::cuda::cvtColor(road, hls_gpu, CV_BGR2HLS);			// hls scale

	cv::Mat hls;
	hls_gpu.download(hls);
	cv::Mat hsl_yellow_mask;
	cv::inRange(hls, cv::Scalar(20, 0, 100), cv::Scalar(30, 255, 255), hsl_yellow_mask);

	cv::Mat grayscale;
	grayscale_gpu.download(grayscale);
	cv::Mat white_gray;
	cv::addWeighted(grayscale, 1, hsl_yellow_mask, 1, 0, white_gray);

	cv::Mat out(cv::Size(grayscale.cols, grayscale.rows), CV_8U);

	LaneMarkDetector laneMarkDetector;

	laneMarkDetector.tau_ = 30;
	laneMarkDetector.verticalOffset_ = 150;
	laneMarkDetector.Process(white_gray, out);

	return out;
}

std::vector<cv::Point2f> ConvertImageToPoints(const cv::Mat& input)
{
	std::vector<cv::Point2f> output;

	unsigned char *raw = (unsigned char*)(input.data);

	int x = 0, y = 0;
	int y_old = 0;

	for (y = 0; y < input.rows; ++y)
	{
		const auto raw = input.ptr(y);
		for (x = 0; x < input.cols; ++x)
		{
			if (raw[x])
			{
				output.emplace_back(cv::Point2f(x, y));
				if(x == 620)
				{
					if(y > y_old)
					y_old = y;
				}
			}
		}
	}
	std::cout << "old : " << output << std::endl;
	if(y_old == 0){
		center = 577;
	}
	else{
		center = sqrt(pow(1.0*(640 - 320), 2) + pow(1.0*y_old - 480, 2));
	}
	return output;
}

cv::Mat LaneDetector::DownsampleImageByHalf(const cv::Mat& input)
{
	cv::Mat minified;
	cv::resize(input, minified, cv::Size(input.cols / 2, input.rows / 2));
	return minified;
}

RoadModel LaneDetector::build(){
		RoadModel roadModel;
		return roadModel;
}

void Distance(cv::Mat& img, int input) {
	cv::Mat road(img.clone());
	cv::Rect roi_out(road.cols * 2 / 5 + 80, road.rows * 3 / 5 + 230, road.cols * 1 / 5 - 120, road.rows * 2/ 5 - 360);

	cv::Mat test(img.clone());
	cv::Mat gray;
	cv::cvtColor(test, gray, CV_BGR2GRAY);
	cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0, 0);

	cv::Mat RoI = gray(roi_out);

	cv::Mat contours;
	cv::Canny(RoI, contours, 100, 200);

	std::vector<cv::Vec2f> lines;
	cv::HoughLines(contours, lines, 10, PI / 180, 80);

	cv::Mat gray_road;
	cv::cvtColor(road, gray_road, CV_BGR2GRAY);
	cv::GaussianBlur(gray_road, gray_road, cv::Size(5, 5), 0, 0);

	cv::Mat RoI_road = gray_road(roi_out);

	cv::Mat contours_road;
	cv::Canny(RoI_road, contours_road, 50, 150);

	std::vector<cv::Vec2f> lines_road;
	cv::HoughLines(contours_road, lines_road, 10, PI / 180, 80);

	cv::Point pt1_avg;
	cv::Point pt2_avg;

	int count = 0;
	float aim = 0;


	if(input == 1)
	{
		std::vector<cv::Vec2f>::const_iterator it = lines.begin();
		while(it != lines.end())
		{
			float rho = (*it)[0];
			float theta = (*it)[1];
			if (theta < PI / 4. || theta > 3.*PI / 4.) {}
			else
			{
				cv::Point pt1(0, rho / sin(theta));
				cv::Point pt2(contours.cols, (rho - contours.cols * cos(theta) / sin(theta)));
				pt1_avg += pt1;
				pt2_avg += pt2;
				count++;
			}
			++it;
		}

		if (count == 0)
		{
			if (input == 1)	distance = 300;
		}
		else
		{
			float timer = clock();
			if (timer < 2000) {}
			else
			{
				if (input == 1)		//if stop
				{
					pt1_avg = pt1_avg / count;
					pt2_avg = pt2_avg / count;

					distance = contours.rows - (pt1_avg.y + pt2_avg.y) / 2;
					std::cout << distance << std::endl;
					cv::line(RoI, pt1_avg, pt2_avg, cv::Scalar(0), 5);
				}
			}
		}

		//cv::imshow("tste", contours);
	}
	else if (input == 3)
	{
		std::vector<cv::Vec2f>::const_iterator it = lines_road.begin();
		while(it != lines_road.end())
		{
			float rho = (*it)[0];
			float theta = (*it)[1];
			if (theta < PI / 4. || theta > 3.*PI / 4.)
			{
				cv::Point pt1(rho / cos(theta), 0);
				cv::Point pt2((rho - contours.rows * sin(theta) / cos(theta)), contours.rows);
				pt1_avg += pt1;
				pt2_avg += pt2;
				count++;
			}
			else
			{
				cv::Point pt1(0, rho / sin(theta));
				cv::Point pt2(contours.cols, (rho - contours.cols * cos(theta) / sin(theta)));
				pt1_avg += pt1;
				pt2_avg += pt2;
				count++;
			}
			++it;
		}

		if (input == 3)		//if park
		{
			pt1_avg = pt1_avg / count;
			pt2_avg = pt2_avg / count;

			int tangent = 0;
			if(pt2_avg.x != pt1_avg.x) tangent = -(pt2_avg.y - pt1_avg.y) / (pt2_avg.x - pt1_avg.x);
			int constant = -pt2_avg.y - tangent * pt2_avg.x;

			if (tangent != 0) aim = -(img.rows + constant) / tangent;

			goal = img.cols / 2 - aim;
		}
	}

	return ;
}

double LaneDetector::FindingCenter() {
	return center;
}

double LaneDetector::stop_line_distance() {
	return distance;
}

int LaneDetector::Inputsign(int input) {
	inputsign = input;

	return 0;
}

int LaneDetector::SetSteering(int steer) {
	steering = steer;
	return 0;
}

double LaneDetector::Parkgoal() {
	return goal;
}

double LaneDetector::FindingCurvature() {
	return curvature;
}

RoadModel LaneDetector::DetectLane(cv::Mat& inputFrame)
{
	cv::cuda::GpuMat inputframe_gpu;
	inputframe_gpu.upload(inputFrame);

	if (inputsign == 1 || inputsign == 3) {
		Distance(inputFrame, inputsign);
	}
	auto roadmodel = build();

	auto lanesPixels = FindPixelsThatMayBelongToLane(inputframe_gpu);
	auto points      = ConvertImageToPoints(lanesPixels);

	//std::cout << steering << std::endl;

	cv::namedWindow("filter", 0);
	cv::resizeWindow("filter",640,480);
	cv::imshow("filter", lanesPixels);
	//cv::setMouseCallback("filter", Mymouse);
	/////////////////////////////////////////


	return roadmodel;
}
}  // namespace vision
