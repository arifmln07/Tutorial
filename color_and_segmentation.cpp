#include <vector>
#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <opencv/highgui.h>
#include <iostream>
#include <list>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/legacy/blobtrack.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/core/core.hpp>
#include <opencv2/video/tracking.hpp>
#include <math.h>

using namespace cv;
using namespace std;

RNG rng(12345);
int max_bouy_radius = 90;

 int main( int argc, char** argv )
 {
    VideoCapture cap("test.avi"); //capture the video from webcam

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return -1;
    }

    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
    namedWindow("Thresholded Image", CV_WINDOW_NORMAL); //create a window called "Control"
    namedWindow("Contour Image", CV_WINDOW_NORMAL); //create a window called "Control"
    namedWindow("Original", CV_WINDOW_NORMAL); //create a window called "Control"
    namedWindow("Contours", CV_WINDOW_NORMAL); //create a window called "Control"



  int iLowH = 0;
 int iHighH = 38;

  int iLowS = 0; 
 int iHighS = 255;

  int iLowV = 137;
 int iHighV = 255;

  //Create trackbars in "Control" window
 createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
 createTrackbar("HighH", "Control", &iHighH, 179);

  createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
 createTrackbar("HighS", "Control", &iHighS, 255);

  createTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
 createTrackbar("HighV", "Control", &iHighV, 255);

   while (true)
    {
        Mat imgOriginal = imread(argv[1], CV_LOAD_IMAGE_UNCHANGED);

		/*
        bool bSuccess = cap.read(imgOriginal); // read a new frame from video

         if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }
        */
        
	GaussianBlur( imgOriginal, imgOriginal, Size( 5, 5 ), 0, 0 );
	GaussianBlur( imgOriginal, imgOriginal, Size( 5, 5 ), 0, 0 );
	GaussianBlur( imgOriginal, imgOriginal, Size( 5, 5 ), 0, 0 );
	
    Mat imgHSV;

   cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
 
  Mat imgThresholded;

   inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
      
  //morphological opening (removes small objects from the foreground)
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );
  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

   //morphological closing (removes small holes from the foreground)
  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) ); 
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

   imshow("Thresholded Image", imgThresholded); //show the thresholded image

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(imgThresholded, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
	
	/// Approximate contours to polygons + get bounding rects and circles
  vector<vector<Point> > contours_poly( contours.size() );
  vector<Rect> boundRect( contours.size() );
  vector<Point2f>center( contours.size() );
  vector<float>radius( contours.size() );

  for( int i = 0; i < contours.size(); i++ ){ 
	  
	  approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
      boundRect[i] = boundingRect( Mat(contours_poly[i]) );
      minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
     }
     
	int largest_bouy_radius = 0;
	int largest_bouy_id = 0;
	int posX = 0;
	int posY = 0;
  /// Draw polygonal contour + bonding rects + circles
  Mat drawing = Mat::zeros( imgThresholded.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ ){
	if (largest_bouy_radius < (int)radius[i]){
		   
		   posX = (int)center[i].x;
		   posY = (int)center[i].y;
		   largest_bouy_radius = (int)radius[i];
		   largest_bouy_id = i;
		}
	}
	
	if(largest_bouy_radius != 0){
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		cout << "######## Posisi X dan Y #########" << endl;
		cout << "Posisi X : " << posX << endl;
		cout << "Posisi Y : " << posY << endl;
		cout << endl;
		cout << endl;
		circle( drawing, center[largest_bouy_id], largest_bouy_radius, color, 2, 8, 0 );

		/// Show in a window
		imshow( "Contours", drawing );
	}
   imshow("Contour Image", imgThresholded); //show the thresholded image
   imshow("Original", imgOriginal); //show the original image

        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
            break; 
       }
    }

   return 0;
}
