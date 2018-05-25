#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
#include<algorithm>

using namespace std;
using namespace cv;

#define COLOR 1
#define GRAY 0
#define VIDEO 2
#define IMAGE 3
class BlackBox
{
private:
	Mat frame;
	VideoCapture cap;
	Size videoSize;

public:
	BlackBox(string fname,int _format);
	BlackBox()	{	}// 기본 생성자
	~BlackBox();
	void drawRect(Mat& _frame, Size _xy,Size _size);
	void processedVideo(int _mode);
	void processedImg();
	void bgr2Gray(Mat &_frame);
	
};

