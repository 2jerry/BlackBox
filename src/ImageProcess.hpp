#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class ImageProcess
{
private:
	Size imgSize;
public:
	ImageProcess(Size _imgSize);
	ImageProcess() { /* 기본 생성자 */ }
	~ImageProcess() { /* 소멸자 */ }
	void ROI2Binary(Mat& _image, Point _xy, Size _size, int _mode);
	void startProcessing(Mat& _frame, int _morphmode);
};


