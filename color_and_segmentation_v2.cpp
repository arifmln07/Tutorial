#include <vector>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/tracking.hpp>

using namespace cv;
using namespace std;

RNG rng(12345);
bool pause = false;

int main( int argc, char** argv ){
	VideoCapture cap("test.avi"); //capture the video from webcam
	
	if (!cap.isOpened()){  // if not success, exit program
		cout << "Cannot open the web cam" << endl;
		return -1;
	}
	
	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
	namedWindow("Thresholded Image", CV_WINDOW_NORMAL); //create a window called "Thresholded Image"
	namedWindow("Contour Image", CV_WINDOW_NORMAL); //create a window called "Contour"
	namedWindow("Original", CV_WINDOW_NORMAL); //create a window called "Original"
	namedWindow("Contours", CV_WINDOW_NORMAL); //create a window called "Contours"
	
	int iLowH 	= 15;	// yellow = 15, red = 166
	int iHighH 	= 29;	// yellow = 29, red = 179
	
	int iLowS	= 90; // lower this to allow more noise, 80 is normal
	int iHighS 	= 184;
	
	int iLowV 	= 138; // lower this to allow more noise
	int iHighV 	= 255;
	
	//Create trackbars in "Control" window
	createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	createTrackbar("HighH", "Control", &iHighH, 179);
	
	createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	createTrackbar("HighS", "Control", &iHighS, 255);
	
	createTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
	createTrackbar("HighV", "Control", &iHighV, 255);
	
	Mat pauseBuffer;
	
	while (true){
		
		Mat imgOriginal	 ;//= imread(argv[1], CV_LOAD_IMAGE_UNCHANGED);
		
		
		if (!pause){
			bool bSuccess = cap.read(imgOriginal); // read a new frame from video
			pauseBuffer = imgOriginal.clone();	// deep copy of imgOriginal
			
			if (!bSuccess){ //if not success, break loop
				
				cout << "Cannot read a frame from video stream" << endl;
				break;
			}
		}
		
		else if (pause){
			imgOriginal = pauseBuffer.clone();
		}
		
		
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
		Point2f center;//( contours.size() );
		float radius;//( contours.size() );
		findContours(imgThresholded, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
		
		Mat drawing = Mat::zeros( imgThresholded.size(), CV_8UC3 );
		Rect bounding_rect;
		int largest_bouy_area = 0;
		int largest_bouy_id = 0;
		int number_of_detected_buoy = 0;
		
		for( int i = 0; i< contours.size(); i++ ){
			double a = contourArea( contours[i],false);  //  Find the area of contour
			if(a > largest_bouy_area){
				largest_bouy_area = a;
				largest_bouy_id = i;                //Store the index of largest contour
				//bounding_rect=boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
				minEnclosingCircle( contours[i], center, radius);
			}
			if(largest_bouy_area > 0){
				number_of_detected_buoy++;
			}
			
		}
		
		cout << "######## Posisi X dan Y #########" << endl;
		cout << "Number of Buoy  : " << number_of_detected_buoy << endl;
		
		if(largest_bouy_area > 0){
			cout << "Posisi X  : " << center.x << endl;
			cout << "Posisi Y  : " << center.y << endl;
			cout << "Bouy Size : " << largest_bouy_area << endl;
			cout << endl;
			cout << endl;
			
			Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
			drawContours( drawing, contours,largest_bouy_id, Scalar(255,255,255), CV_FILLED, 8, hierarchy ); // Draw the largest contour using previously stored index.
			//rectangle(imgOriginal, bounding_rect,  color,4, 8,0);
			circle( imgOriginal, center, radius, color, 4, 8, 0 );
		}
		imshow("Contours", drawing );
		imshow("Contour Image", imgThresholded); //show the thresholded image after findContours()
		imshow("Original", imgOriginal); //show the original image
		
		char input_keyboard = waitKey(30);
		if (input_keyboard == 27){ //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
			
			cout << "esc key is pressed by user" << endl;
			break;
		}
		
		else if (input_keyboard == 'p'){ //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
			
			pause = !pause;
		}
	}
	
	return 0;
}
