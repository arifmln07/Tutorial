#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;
Mat Original, Gray, HSV, Threshold;
int main()
{
	VideoCapture cap(0);
	int Low=233, High=255;
	while(1)
	{
		cap.read(Original);
		cvtColor(Original, Gray, COLOR_BGR2GRAY);
		cvtColor(Original, HSV, COLOR_BGR2HSV);
		inRange(Gray,Low,High,Threshold);
		imshow("Original",Original);
		imshow("HSV", HSV);
		imshow("Grayscale", Gray);
		imshow("Threshold Image",Threshold);
		waitKey(30);
	}
}
