#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int main()
{
    VideoCapture vc0(0);
    VideoCapture vc1(1);
    if(!vc0.isOpened()) {
        printf("Cannot open VideoCapture 0!");
        return 1;
    }
    if(!vc1.isOpened()) {
        printf("Cannot open VideoCapture 1!");
        return 1;
    }

    Mat img0;
    Mat img1;

    //cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 640);
    //cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 480);

    //cvNamedWindow("OpenCvCamtest", 0);
    //cvResizeWindow("OpenCvCamtest", 640, 480);

	namedWindow("cam0", WINDOW_NORMAL);
	namedWindow("cam1", WINDOW_NORMAL);
	resizeWindow("cam0", 500, 500);
	resizeWindow("cam1", 500, 500);
    while(1) {
        vc0 >> img0;
        vc1 >> img1;
        if(img0.empty()) continue;
        if(img1.empty()) continue;

		moveWindow("cam1", 0, 250);
        imshow("cam0", img0);
        imshow("cam1", img1);

        if(cvWaitKey(30) == 'q')
            break;
    }

    destroyAllWindows();

    return 0;
}
