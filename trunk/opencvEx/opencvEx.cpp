// opencvEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <cv.h>
#include <highgui.h>

#ifdef _DEBUG
#pragma comment(lib, "opencv_highgui2411d.lib")
#else
#pragma comment(lib, "opencv_highgui2411.lib")
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	IplImage* pImage = 0;                        
	pImage = cvLoadImage("test1.png", 1);

	if(pImage == NULL)
		return 0;

	cvNamedWindow("PNG Viewer", 1);    
	cvShowImage("PNG Viewer", pImage);
	cvWaitKey(0);                     

	cvDestroyWindow("PNG Viewer");

	return 0;
}