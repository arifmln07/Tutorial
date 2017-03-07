#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

 int main( int argc, char** argv )
 {
	
	// open the chessboard calibration configuration file
	FileStorage fs("camera.yml", FileStorage::READ);
	Mat intrinsics, distortion;
	fs["camera_matrix"] >> intrinsics;
	fs["distortion_coefficients"] >> distortion;

    VideoCapture cap(1); //capture the video from webcam

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return -1;
    }

    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	 int iLowH = 162;
	 int iHighH = 179;

	 int iLowS = 96; 
	 int iHighS = 255;

	 int iLowV = 197;
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
        Mat imgOriginal;
        Mat imgUndistort;

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video



         if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }

  // to undistort image using configuration obtain from chessboard calibration
  undistort(imgOriginal,imgUndistort,intrinsics,distortion);
  Mat imgHSV;

  cvtColor(imgUndistort, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
 
  Mat imgThresholded;

  inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
      
  //morphological opening (removes small objects from the foreground)
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

  //morphological closing (removes small holes from the foreground)
  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

  //Calculate the moments of the thresholded image
  Moments oMoments = moments(imgThresholded);

  double dM01 = oMoments.m01;
  double dM10 = oMoments.m10;
  double dArea = oMoments.m00;

  // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
  if (dArea > 10000)
  {
   //calculate the position of the object
   int posX = dM10 / dArea;
   int posY = dM01 / dArea;
   posX = posX - 15;		// set the 0 cm 15 pixel to right of the most left pixel
   
   // ratio gathered from first calibration
   double initRatio = 0.06;	// ratio between real distance and pixel, which is 30cm by 500px = 0.06
   double initCameraDistToROV = 30; //distance between camera and the rov when first calibration
  
   double currentCameraDistToROV = 489; // change this to the current distance from the camera lens to the object in cm
   double currentRatio = (currentCameraDistToROV/initCameraDistToROV)*initRatio;
   double realDist = posX * currentRatio;
   
   cout << endl;
   cout << "Posisi X   : " << posX << " pixel" << endl;
   cout << "Posisi Y   : " << posY << " pixel" << endl;
   cout << "Posisi AUV : " << realDist << " cm" << endl;
   
  }

  imshow("Thresholded Image", imgThresholded); //show the thresholded image
  imshow("Undistort", imgUndistort); //show the undistort image

        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
            break; 
       }
    }

   return 0;
}
