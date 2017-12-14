#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int main()
{
    IplImage* dest = NULL;
    IplImage* image = NULL;
    IplImage* image2 = NULL;
    CvCapture* capture = cvCaptureFromCAM(0);

    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 640);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 480);

    cvNamedWindow("OpenCvCamtest", 0);
    cvResizeWindow("OpenCvCamtest", 640, 480);

    int width;
    int height;
    int count = 0;
    bool flag = false;

    while(1) {

        cvGrabFrame(capture);

        ++count;
        if(count&3) continue;

        image = cvRetrieveFrame(capture);

        width = image->widthStep;
        height = image->height;

        cvReleaseImage(&dest);
        dest = cvCloneImage(image);

        if(flag) {
            for(int i=0; i<height; i++) {
                for(int j=0; j<width; j+=image->nChannels) {
                    int r1 = image->imageData[j + i*width];
                    int r2 = image2->imageData[j + i*width];
                    if(r1-r2 > 32 || r1-r2 < -32) {
                        //dest->imageData[j + i*width] = 255;
                        //dest->imageData[j + i*width + 1] = 255;
                        //dest->imageData[j + i*width + 2] = 255;
                    }
                    else {
                        dest->imageData[j + i*width] = 0;
                        dest->imageData[j + i*width + 1] = 0;
                        dest->imageData[j + i*width + 2] = 0;
                    }
                }
            }
        }

        cvReleaseImage(&image2);
        image2 = cvCloneImage(image);
        flag = true;
        cvShowImage("OpenCvCamtest", dest);
        if(cvWaitKey(10) == 'q')
            break;
    }

    cvReleaseCapture(&capture);
    cvDestroyWindow("OpenCvCamtest");
    return 0;
}
