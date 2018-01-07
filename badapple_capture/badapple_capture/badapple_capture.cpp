// badapple_capture.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2\opencv.hpp> 
#include <opencv2/video/video.hpp>
#include "highgui.h"
#include <iostream>
using namespace cv;
using namespace std;

int main()
{
	VideoCapture capture("badapple.avi");
	if (!capture.isOpened()) {
		cout << "fail to open!" << endl;
	}
	long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "整个视频共" << totalFrameNumber << "帧" << endl;
	double rate = capture.get(CV_CAP_PROP_FPS);
	cout << "帧率为:" << rate << endl;
//	VideoWriter writer;

//	Size videoSize(capture.get(CV_CAP_PROP_FRAME_WIDTH), capture.get(CV_CAP_PROP_FRAME_HEIGHT));
//	int fps = capture.get(CV_CAP_PROP_FPS);
//	int fps = 30;
	Mat frame;
//	capture >> frame;
//	writer.open("tree.avi", CV_FOURCC('X', 'V', 'I', 'D'), fps, videoSize);

	stringstream ss;
	int num=0;
	string str;
	char imagename[100];

	while (capture.read(frame))
	{
//		capture >> frame;
//		if (frame.empty())
//			break;
//		writer << frame;
		ss << num;
		ss >> str;
		sprintf_s(imagename, "%s%d%s","./image/badapple_", ++num,".bmp");
		
		imwrite(imagename, frame);
		
//		waitKey(30);
	}
	capture.release();
    return 0;
}