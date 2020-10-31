#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <ctime>

#include "IPM.h"

using namespace cv;
using namespace std;

int main( int _argc, char** _argv )
{
	// Images
	Mat inputImg, inputImgGray;
	Mat outputImg;
	
	if( _argc != 2 )
	{
		cout << "Usage: ipm.exe <videofile>" << endl;
		return 1;
	}

	// Video
	string videoFileName = _argv[1];	
	cv::VideoCapture video;
	if( !video.open(videoFileName) )
		return 1;

	// Show video information
	int width = 0, height = 0, fps = 0, fourcc = 0;	
	width = static_cast<int>(video.get(CAP_PROP_FRAME_WIDTH));
	height = static_cast<int>(video.get(CAP_PROP_FRAME_HEIGHT));
	fps = static_cast<int>(video.get(CAP_PROP_FPS));
	fourcc = static_cast<int>(video.get(CAP_PROP_FOURCC));

	cout << "Input video: (" << width << "x" << height << ") at " << fps << ", fourcc = " << fourcc << endl;
	
	// The 4-points at the input image	
	vector<Point2f> originalPoints;
	originalPoints.push_back( Point2f(0, height) );
	originalPoints.push_back( Point2f(width, height) );
	originalPoints.push_back( Point2f(width/2+30, 270) );
	originalPoints.push_back( Point2f(width/2-50, 270) );

	// The 4-points correspondences in the destination image
	vector<Point2f> destinationPoints;
	destinationPoints.push_back( Point2f(width/2-100, height) );
	destinationPoints.push_back( Point2f(width/2+70, height) );
	destinationPoints.push_back( Point2f(width/2+30, 270) );
	destinationPoints.push_back( Point2f(width/2-50, 270) );
		
	// IPM object
	IPM ipm( Size(width, height), Size(width, height), originalPoints, destinationPoints );
	
	// Main loop
	int frameNum = 0;
	for( ; ; )
	{
		// printf("FRAME #%6d ", frameNum);
		fflush(stdout);
		frameNum++;

		// Get current image		
		video >> inputImg;
		if( inputImg.empty() )
			break;

		 // Color Conversion
		 if(inputImg.channels() == 3)		 
			 cvtColor(inputImg, inputImgGray, cv::COLOR_BGR2GRAY);				 		 
		 else	 
			 inputImg.copyTo(inputImgGray);			 		 

		 // Process
		 ipm.applyHomography( inputImg, outputImg );		 
		 ipm.drawPoints(originalPoints, inputImg );

		 // View		
		 imshow("Input", inputImg);
		 imshow("Output", outputImg);
		 waitKey(0);
	}

	return 0;	
}		
