#include <vector>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/tracking.hpp>

using namespace cv;
using namespace std;

RNG rng(12345);
bool pause = false;
void imageProcessing(Mat imgInput, int low_hue, int high_hue, int minimum_area, Mat& detected_blob);
	
int main( int argc, char** argv ){
	
	Mat detected_blob_data;
	Mat imageRead	 = imread(argv[1], CV_LOAD_IMAGE_UNCHANGED);
	imageProcessing(imageRead, 15, 29,0, detected_blob_data);
	
	imshow("Contours", detected_blob_data );
	waitKey(0);
	
	
	return 0;
}

void imageProcessing(Mat imgInput, int low_hue, int high_hue, int minimum_area, Mat& detected_blob){
	
	Mat imgOriginal = imgInput.clone();
	
	namedWindow("Thresholded Image", CV_WINDOW_NORMAL); //create a window called "Thresholded Image"
	namedWindow("Contour Image", CV_WINDOW_NORMAL); //create a window called "Contour"
	namedWindow("Original", CV_WINDOW_NORMAL); //create a window called "Original"
	namedWindow("Contours", CV_WINDOW_NORMAL); //create a window called "Contours"
	
	int iLowH 	= low_hue;	// yellow = 15, red = 166
	int iHighH 	= high_hue;	// yellow = 29, red = 179
	
	int iLowS	= 90; // lower this to allow more noise, 80 is normal
	int iHighS 	= 184;
	
	int iLowV 	= 138; // lower this to allow more noise
	int iHighV 	= 255;
	
	GaussianBlur( imgOriginal, imgOriginal, Size( 5, 5 ), 0, 0 );
	
	Mat imgHSV;
	cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
	
	Mat imgThresholded;
	
	inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
	
	//morphological opening (removes small objects from the foreground)
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	
	//morphological closing (removes small holes from the foreground)
	dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	
	imshow("Thresholded Image", imgThresholded); //show the thresholded image
	
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Point2f center( contours.size() );
	float radius( contours.size() );
	findContours(imgThresholded, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
	
	int largest_bouy_area = 0;
	int largest_bouy_id = 0;
	
	for( int i = 0; i< contours.size(); i++ ){
		double a = contourArea( contours[i],false);  //  Find the area of contour
		if(a > largest_bouy_area && a > minimum_area){
			largest_bouy_area = a;
			largest_bouy_id = i;                //Store the index of largest contour
			minEnclosingCircle( contours[i], center, radius);
		}
	}
	
	
	Mat drawing = Mat::zeros( imgThresholded.size(), CV_8UC3 );
	if(largest_bouy_area > minimum_area){
		cout << "######## Posisi X dan Y #########" << endl;
		cout << "Posisi X  : " << center.x << endl;
		cout << "Posisi Y  : " << center.y << endl;
		cout << "Bouy Size : " << largest_bouy_area << endl;
		cout << endl;
		cout << endl;
		drawContours( drawing, contours,largest_bouy_id, Scalar(255,255,255), CV_FILLED, 8, hierarchy ); // Draw the largest contour using previously stored index.
	}
	
	
	Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
	circle( imgOriginal, center, radius, color, 4, 8, 0 );
	
	detected_blob = drawing.clone();
	imshow("Contour Image", imgThresholded); //show the thresholded image after findContours()
	imshow("Original", imgOriginal); //show the original image
		
}
