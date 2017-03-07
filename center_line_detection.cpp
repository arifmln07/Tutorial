#include <vector>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/video/tracking.hpp>

using namespace cv;
using namespace std;

bool pause = false;
int imageProcessing(Mat imgInput, int low_hue, int high_hue, int minimum_area, int *detected_center_x, int *detected_center_y, double *contour_area, double* detected_radius);
double calculateCenterLine(int first_x, int second_x);

int first_buoy_x	= 0;
int first_buoy_y 	= 0;
double first_buoy_area	= 0;
double first_radius_buoy	= 0;
int second_buoy_x		= 0;
int second_buoy_y 	= 0;
double second_buoy_area		= 0;
double second_buoy_radius	= 0;
	
int main( int argc, char** argv ){
	
	Mat imageRead	 = imread(argv[1], CV_LOAD_IMAGE_UNCHANGED);
	Mat imageResult;

	imageResult = imageRead.clone();

	int buoy_number = imageProcessing(imageRead, 15, 29,0, &first_buoy_x, &first_buoy_y, &first_buoy_area, &first_radius_buoy);
	
	int buoy_number_r = imageProcessing(imageRead, 166, 179,0, &second_buoy_x, &second_buoy_y, &second_buoy_area, &second_buoy_radius);
	
	cout << "######## Posisi Tengah #########" << endl;
	cout << "Posisi X  : " << first_buoy_x << endl;
	cout << "Posisi X_R : " << second_buoy_x << endl;
	cout << "Center Buoy : " << calculateCenterLine(first_buoy_x,second_buoy_x) << endl;
	
	waitKey(0);	
	
	return 0;
}

int imageProcessing(Mat imgInput, int low_hue, int high_hue, int minimum_area, int *detected_center_x, int *detected_center_y, double *contour_area, double* detected_radius){
	
	Mat imgOriginal = imgInput.clone();
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
	
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Point2f center;
	float radius;
	findContours(imgThresholded, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
	
	int largest_bouy_area = 0;
	int largest_bouy_id = 0;
	int number_of_detected_buoy = 0;
	for( int i = 0; i< contours.size(); i++ ){
		double a = contourArea( contours[i],false);  //  Find the area of contour
		if(a > largest_bouy_area && a > minimum_area){
			largest_bouy_area = a;
			largest_bouy_id = i;                //Store the index of largest contour
			minEnclosingCircle( contours[i], center, radius);
		}
		if(largest_bouy_area > minimum_area){
			number_of_detected_buoy++;
		}
		
	}
	if(largest_bouy_area > minimum_area){
		
		*detected_center_x	=	(int)center.x;
		*detected_center_y	=	(int)center.y;
		*contour_area		=	largest_bouy_area;
		*detected_radius	=	radius;
	}
	
	return number_of_detected_buoy;
}

double calculateCenterLine(int first_x, int second_x){
	
	int left_x, right_x;
	if (first_x > second_x){
		left_x = second_x;
		right_x = first_x;
	}
	
	else{
		left_x = first_x;
		right_x = second_x;
	}
	
	double center_line;
	center_line = abs(left_x - right_x)/2 + left_x;
	
	return center_line;
	
}
