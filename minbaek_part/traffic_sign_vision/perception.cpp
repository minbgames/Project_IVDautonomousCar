#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(void)
{
	VideoCapture capture(0);

	if (!capture.isOpened()) {
		std::cerr << "Could not open camera" << std::endl;
		return 0;
	}

	//setting
	Mat frame;
	Mat hsv_frame;
	Mat dst_frame;
	Mat blur_frame;

	// red value range
	Scalar lowerb_red(170, 50, 0);
	Scalar upperb_red(179, 255, 255);

	// blue value range
	Scalar lowerb_blue(75, 50, 0);
	Scalar upperb_blue(130, 255, 255);

	//set contours
	int mode = RETR_EXTERNAL;
	int method = CHAIN_APPROX_SIMPLE;
	vector<vector<Point> > contours;

	//set blur
	int blur_size = 15;

	while (true)
	{
		bool frame_valid = true;

		try {
			capture >> frame; // get a new frame from webcam
		}
		catch (Exception& e) {
			std::cerr << "Exception occurred. Ignoring frame... " << e.err
				<< std::endl;
			frame_valid = false;
		}
		if (frame_valid) { // input code!!!!!!!!

			cvtColor(frame, hsv_frame, COLOR_BGR2HSV);
			inRange(hsv_frame, lowerb_red, upperb_red, dst_frame);
			medianBlur(dst_frame,blur_frame,blur_size);
			findContours(blur_frame,contours,noArray(),mode,method);
			for (int k = 0; k < contours.size(); k++)
			{
				Scalar color(255,0,0);
				drawContours(frame, contours, k, color, 4);

				for (int j = 0; j < contours[k].size(); j++)
				{
					Point pt = contours[k][j];
				}
			}
			imshow("dst_frame",dst_frame);
			imshow("blur_frame",blur_frame);
			imshow("window", frame);
		}

		/*when put key, program stop*/
		if (waitKey(1) >= 0) break;
	}
	return 0;
}
