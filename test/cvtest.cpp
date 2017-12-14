#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	Mat image;

	image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	if(!image.data) {
		cout <<  "imread() error" << endl;
		return -1;
	}

	//namedWindow("OCV test", WINDOW_AUTOSIZE);
	imshow("Window", image);
	waitKey(0);

	return 0;
}
