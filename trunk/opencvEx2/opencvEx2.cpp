// opencvEx2.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <cv.h>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;


#ifdef _DEBUG
#pragma comment(lib, "opencv_highgui2411d.lib")
#pragma comment(lib, "opencv_core2411d.lib")
#else
#pragma comment(lib, "opencv_highgui2411.lib")
#pragma comment(lib, "opencv_core2411d.lib")
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	CvCapture* capture = 0;
	Mat frame, frameCopy, image;

	capture = cvCaptureFromCAM(0); //0=default, -1=any camera, 1..99=your camera
	if (!capture) cout << "No camera detected" << endl;

	cvNamedWindow("result", 1);

	if (capture)
	{
		cout << "In capture ..." << endl;
		for (;;)
		{
			IplImage* iplImg = cvQueryFrame(capture);
			frame = iplImg;
			if (frame.empty())
				break;
			if (iplImg->origin == IPL_ORIGIN_TL)
				frame.copyTo(frameCopy);
			else
				flip(frame, frameCopy, 0);

			if (waitKey(10) >= 0)
				cvReleaseCapture(&capture);
		}

		waitKey(0);

		cvDestroyWindow("result");

	}

	return 0;
}