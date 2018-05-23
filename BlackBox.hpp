#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

#define COLOR 1
#define GRAY 0

class BlackBox
{
private:
	Mat frame;
	VideoCapture cap;
	Size videoSize;

public:
	BlackBox(string fname);
	BlackBox(); // 기본 생성자
	~BlackBox();
	void drawRect(Mat& _frame, Size _xy,Size _size);
	void playVideo(int _mode);
	void bgr2Gray(Mat &_frame);
	
};

