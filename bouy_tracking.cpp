#include <vector>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/tracking.hpp>

using namespace cv;
using namespace std;

RNG rng(12345);
int imageProcessing(Mat imgInput, int low_hue, int high_hue, int minimum_area, int *detected_center_x, int *detected_center_y, double *contour_area, double* detected_radius);

	int iLowH 	= 166;	// yellow = 15, red = 166
	int iHighH 	= 179;	// yellow = 29, red = 179
	
	int iLowS	= 90; // lower this to allow more noise, 80 is normal
	int iHighS 	= 184;
	
	int iLowV 	= 138; // lower this to allow more noise
	int iHighV 	= 255;
	
	

int main( int argc, char** argv ){
	
	VideoCapture cap("test.avi"); //capture the video from webcam
	
	if (!cap.isOpened()){  // if not success, exit program
		cout << "Cannot open the web cam" << endl;
		return -1;
	}
	
	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
	//Create trackbars in "Control" window
	createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	createTrackbar("HighH", "Control", &iHighH, 179);
	
	createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	createTrackbar("HighS", "Control", &iHighS, 255);
	
	createTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
	createTrackbar("HighV", "Control", &iHighV, 255);
	
	
	namedWindow("Image", CV_WINDOW_NORMAL); //create a window called "Thresholded Image"
	namedWindow("Threshold", CV_WINDOW_NORMAL); //create a window called "Thresholded Image"
		
	while (true){
		
		Mat imageRead = imread(argv[1], CV_LOAD_IMAGE_UNCHANGED);
	
		//bool bSuccess = cap.read(imageRead); // read a new frame from video
		//if (!bSuccess){ //if not success, break loop
			
		//	cout << "Cannot read a frame from video stream" << endl;
		//	break;
		//}
		
		int center_buoy_x	= 0;
		int center_buoy_y 	= 0;
		double buoy_area	= 0;
		double radius_buoy	= 0;
		
		int buoy_number = imageProcessing(imageRead, 15, 29,0, &center_buoy_x, &center_buoy_y, &buoy_area, &radius_buoy);
		
		cout << "######## Posisi Yellow #########" << endl;
		cout << "Detected Buoy : " << buoy_number << endl;
		cout << "Posisi X  : " << center_buoy_x << endl;
		cout << "Posisi Y  : " << center_buoy_y << endl;
		cout << "Bouy Size : " << buoy_area << endl;
		cout << endl;
		cout << endl;
		
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		Point2f circle_center;
		circle_center.x = center_buoy_x;
		circle_center.y = center_buoy_y;
		circle( imageRead, circle_center, radius_buoy, color, 4, 8, 0 );
		
		imshow("Image", imageRead );
		
		if (waitKey(30) == 27){ //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
			
			cout << "esc key is pressed by user" << endl;
			break;
		}
		
	}
	
	return 0;
}

int imageProcessing(Mat imgInput, int low_hue, int high_hue, int minimum_area, int *detected_center_x, int *detected_center_y, double *contour_area, double* detected_radius){
	
	Mat imgOriginal = imgInput.clone();
		
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
	
	imshow("Threshold", imgThresholded );
	
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
