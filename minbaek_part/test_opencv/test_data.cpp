#include "opencv2/opencv.hpp"
#include <time.h>

using namespace cv;
using namespace std;

int main(void)
{
	VideoCapture capture(0);

	if (!capture.isOpened()) {
		std::cerr << "Could not open camera" << std::endl;
		return 0;
	}

	bool isthisgoodmatch = true;
	Mat frame;
	Mat frame_gray;
	Mat hsv;
	Mat upper_Hue;
	Mat lower_Hue;
	Mat contours;
	namedWindow("window");

	Mat src_speed_30 = imread("study_img/baboon1.jpg", IMREAD_GRAYSCALE);
	if (src_speed_30.empty())
		return -1;

    capture >> frame;

		if(frame.empty()) return 0;

    cvtColor(frame, frame_gray, CV_BGR2GRAY);

    cout << "frame_gray depth=" << frame_gray.depth() << ","<<" channels="<<frame_gray.channels()<<endl;
    cout << "frame depth=" << frame.depth() << ","<<" channels="<<frame.channels()<<endl;
    cout << "src_speed_30 depth=" << src_speed_30.depth() << ","<<" channels="<<src_speed_30.channels()<<endl;

    while(1){if(waitKey(0)>0) break;}



}
