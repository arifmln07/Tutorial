#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;
Mat Original, Gray, Threshold;
int main()
{
	VideoCapture cap(0);
	int Low = 233, High = 255;
	while(1)
	{
		cap.read(Original);
		cvtColor(Original, Gray, COLOR_BGR2GRAY);
		inRange(Gray, Low, High, Threshold);
		imshow("Threshold", Threshold);
		Moments oMoments = moments(Threshold);
		double dM01 = oMoments.m01;
		double dM10 = oMoments.m10;
		double dArea = oMoments.m00;
		if(dArea > 100)
		{
			int posX = dM10/dArea;
			int posY = dM01/dArea;
			cout << "Posisi: (" <<posX <<","<<posY<<")"<<endl;
		}
		waitKey(30);
	}
} 
