#pragma once
#include "ImageProcess.hpp"

#define RED Scalar(0,0,255)
#define GREEN Scalar(0,2,255)
#define RED Scalar(0,0,255)
#define COLOR 1
#define GRAY 0

class FindLicencePlates 
{
private:
	Mat frame;
	VideoCapture cap;
	Size videoSize;
	ImageProcess *p;

public:
	int PLAY = 1;
	FindLicencePlates(string fname);
	FindLicencePlates() { /* 기본 생성자 */ }
	~FindLicencePlates() 
	{
		cap.release();
		destroyAllWindows();
		delete p; 
	}
	void drawRect(Mat& _frame, Point _xy, Size _size);
	void playVideo(Mat &temp_frame,int _mode = COLOR);
	bool numberDetect(Size* _point);
	Size* findRectPoint(vector<Point> _poly);
	void detectRectangle(Mat& _frame);
};
