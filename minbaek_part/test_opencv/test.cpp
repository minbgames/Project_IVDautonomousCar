#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;
int main()
{
	// create an input image
	Mat srcImage = Mat::zeros(512, 512, CV_8UC1);
	rectangle(srcImage, Point(50, 100), Point(200, 400), Scalar::all(255), -1);
	rectangle(srcImage, Point(80, 150), Point(180, 350), Scalar::all(0), -1);

	rectangle(srcImage, Point(250, 100), Point(450, 400), Scalar::all(255), -1);
	rectangle(srcImage, Point(280, 150), Point(420, 350), Scalar::all(0), -1);
	rectangle(srcImage, Point(320, 200), Point(380, 300), Scalar::all(255), -1);

	Mat dstImage(srcImage.size(), CV_8UC3);
	cvtColor(srcImage, dstImage, COLOR_GRAY2BGR);

	//	int mode = RETR_EXTERNAL;
	int mode = RETR_LIST;

	int method = CHAIN_APPROX_SIMPLE;
	//	int method = CHAIN_APPROX_NONE;

	vector<vector<Point> > contours;
	findContours(srcImage, contours, noArray(), mode, method);
	cout << "contours.size()=" << contours.size() << endl;

	//	drawContours(dstImage, contours, -1, Scalar(0,0,255), 2);
	for (int k = 0; k < contours.size(); k++)
	{
		Scalar color(rand() & 255, rand() & 255, rand() & 255);
		drawContours(dstImage, contours, k, color, 4);

		cout << " contours[" << k << "].size() = " << contours[k].size() << endl;
		for (int j = 0; j < contours[k].size(); j++)
		{
			Point pt = contours[k][j];
			cout << " pt[" << j << "] = " << pt << endl;
		}
	}
	imshow("dstImage", dstImage);
	waitKey();
	return 0;
}
