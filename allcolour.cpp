#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>

using namespace cv;
using namespace std;

Mat imgOriginal, imgHSV, imgThresholded, imgErode, imgDilate;

int main( int argc, char** argv )
{
	VideoCapture cap(0);//webcam
	if ( !cap.isOpened() )  // jika tidak terbaca, exif
    {
         cout << "webcam tidak terbaca" << endl;
         return -1;
    }
	cap.read(imgOriginal);
    namedWindow("ColourTracking", CV_WINDOW_AUTOSIZE);
	
    int LowH = 170;//warna merah
	int HighH = 179;//warna merah

	int LowS = 150; 
	int HighS = 255;

	int LowV = 60;
	int HighV = 255;
	
	createTrackbar("LowH", "ColourTracking", &LowH, 255); //Hue (0 - 255)
	createTrackbar("HighH", "ColourTracking", &HighH, 255);

	createTrackbar("LowS", "ColourTracking", &LowS, 255); //Saturation (0 - 255)
	createTrackbar("HighS", "ColourTracking", &HighS, 255);

	createTrackbar("LowV", "ColourTracking", &LowV, 255);//Value (0 - 255)
	createTrackbar("HighV", "ColourTracking", &HighV, 255);
	
	printf("Hue values of basic colors\n Orange  0-22\n Yellow 22- 38\n Green 38-75\n Blue 75-130\n Violet 130-160\n Red 160-179\n");
	
	while (true)
    {
        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal); // memebaca frame baru dari video

         if (!bSuccess) //jika tidak sukses, menghentikan loop
        {
             cout << "tidak bisa membaca frame dari video" << endl;
             break;
        }

		Mat imgHSV;

		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //mengubah frame BGR jadi HSV
 
		Mat imgThresholded;

		inRange(imgHSV, Scalar(LowH, LowS, LowV), Scalar(HighH, HighS, HighV), imgThresholded);
      
		//hapus objek kecil
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
		dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

		//hapus hole
		dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

		imshow("Threshold", imgThresholded);

		imshow("Original", imgOriginal); 

        if (waitKey(30) == 27) //exit esc
        {
            cout << "esc di tekan" << endl;
            break; 
        }
   }
   return 0;
}
