#include <iostream>
#include "opencv2/highgui/highgui.hpp"
using namespace std;
using namespace cv;
int main()
{
	Mat img;
	img = imread("baboon.jpg", CV_LOAD_IMAGE_COLOR);
	namedWindow("MyWindow",CV_WINDOW_AUTOSIZE);
	imshow("MyWindow", img);
	waitKey(0);
	return 0;
}
