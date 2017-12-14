#include "opencv2/opencv.hpp"
#include <time.h>

using namespace cv;
using namespace std;

inline void mix_channels(cv::Mat const &src, cv::Mat &dst, std::initializer_list<int> from_to)
{
	cv::mixChannels(&src, 1, &dst, 1, std::begin(from_to), from_to.size() / 2);
}

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
	namedWindow("window");

	Mat src_speed_30 = imread("img/s_30.JPG", IMREAD_GRAYSCALE);
	if (src_speed_30.empty())
		return -1;

	vector<KeyPoint> keypoints1, keypoints2;
	Mat descriptors1, descriptors2;

	Ptr<ORB> orbF = ORB::create(1000);

	orbF->detectAndCompute(src_speed_30, noArray(), keypoints1, descriptors1);
	cout << "keypoints1.size()=" << keypoints1.size() << endl;

	while (true) {
		clock_t start = clock();
		capture >> frame;

		if(frame.empty()) return 0;

		cvtColor(frame, frame_gray, CV_BGR2GRAY);

		orbF->detectAndCompute(frame_gray, noArray(), keypoints2, descriptors2);

		int k = 2;
		Mat indices;
		Mat dists;

		try{
			flann::Index flannIndex(descriptors2, flann::LshIndexParams(12, 20, 2),cvflann::FLANN_DIST_HAMMING);
			flannIndex.knnSearch(descriptors1, indices, dists, k, flann::SearchParams());
		}
		catch(Exception& e){
			continue;
		}


		vector< DMatch > goodMatches;
		float nndrRatio = 0.6f;
		for (int i = 0; i < descriptors1.rows; i++)
		{
			float d1, d2;
			d1 = (float)dists.at<int>(i, 0);
			d2 = (float)dists.at<int>(i, 1);

			if (indices.at<int>(i, 0) >= 0 && indices.at<int>(i, 1) >= 0 &&
				d1 <= nndrRatio*d2)
			{
				//			cout << "i=" << i << ",  d1=" << d1 <<endl;
				DMatch match(i, indices.at<int>(i, 0), d1);
				goodMatches.push_back(match);
			}
		}
		cout << "goodMatches.size()=" << goodMatches.size() << endl;


		if (goodMatches.size() < 20)
		{
			isthisgoodmatch = false;
		}
		else
		{
			isthisgoodmatch = true;
			cout << "speed is 30"<< endl;
		}

		if (isthisgoodmatch) {

			// draw good_matches
			Mat imgMatches;
			drawMatches(src_speed_30, keypoints1, frame_gray, keypoints2,
				goodMatches, imgMatches, Scalar::all(-1), Scalar::all(-1),
				vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS); //DEFAULT
																		   //	imshow("Good Matches", imgMatches);

			vector<Point2f> obj;
			vector<Point2f> scene;
			for (int i = 0; i < goodMatches.size(); i++)
			{
				// Get the keypoints from the good matches
				obj.push_back(keypoints1[goodMatches[i].queryIdx].pt);
				scene.push_back(keypoints2[goodMatches[i].trainIdx].pt);
			}
			Mat H = findHomography(obj, scene, RANSAC); //CV_RANSAC

			vector<Point2f> objP(4);
			objP[0] = Point2f(0, 0);
			objP[1] = Point2f(src_speed_30.cols, 0);
			objP[2] = Point2f(src_speed_30.cols, src_speed_30.rows);
			objP[3] = Point2f(0, src_speed_30.rows);

			vector<Point2f> sceneP(4);
			perspectiveTransform(objP, sceneP, H);

			// draw sceneP in imgMatches
			for (int i = 0; i < 4; i++)
				sceneP[i] += Point2f(src_speed_30.cols, 0);
			for (int i = 0; i < 4; i++)
				line(imgMatches, sceneP[i], sceneP[(i + 1) % 4], Scalar(255, 0, 0), 4);
			imshow("window", imgMatches);
		}
		else
		{
			imshow("window", frame_gray);
		}

		clock_t finish = clock();
		cout << (double)(finish - start)/ CLOCKS_PER_SEC << endl;
		if (waitKey(1) >= 0) break;
	}
	// VideoCapture automatically deallocate camera object
	return 0;
}
