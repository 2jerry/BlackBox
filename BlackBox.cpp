#include "BlackBox.hpp"

BlackBox::BlackBox(string fname)
{
	VideoCapture _cap(fname);
	Mat temp;
	cap = _cap;

	if (!cap.isOpened())
	{
		cout << "Error opening video stream or file" << endl;
		return;
	}

	cap >> temp;
	//resize(temp, temp, Size(), 0.5, 0.5);
	videoSize = temp.size();
	cout << "width : " << videoSize.width << ", height : " << videoSize.height << endl;
}

BlackBox::BlackBox()
{
}

BlackBox::~BlackBox()
{
}

void BlackBox::drawRect(Mat& _frame,Size _xy, Size _size)
{
	rectangle(_frame, Rect(_xy.width, _xy.height,\
		_size.height,_size.width), Scalar(0, 0, 255), 2);
}

void BlackBox::playVideo(int _mode)
{
	while (1)
	{
		
		cap >> frame;
		//resize(frame, frame, Size(), 0.5, 0.5);
		if (frame.empty())	break;

		if (!_mode) // gray 일때만
			bgr2Gray(frame);


		Mat _ff;

		//Mat kernel(7, 7, CV_8U);
		//morphologyEx(frame, frame, MORPH_TOPHAT, kernel);

		//for (int i = 0; i < videoSize.height / 2.2; i++)
		//{
		//	for (int j = 0; j < videoSize.height / 2.2; j++)
		//	{
		//		
		//		frame.
		//		bool white_pixel = (B >= 200 && G >= 200 && R >= 200);
		//		if (white_pixel == false)
		//		{
		//			//drawRect(_ff,Size(j,i), Size(10, 10));// 사각형 
		//		}

		//	}
		//}

		
		line(frame, Point(0, videoSize.height / 2.2), Point(videoSize.width, videoSize.height / 2.2), \
			Scalar(0, 255, 0), 3);
		
		//drawRect(frame, videoSize/2, Size(30, 30));// 사각형 
		//drawRect(frame, Size(30, 30));// 사각형 

		imshow("Frame", frame);

		char c = (char)waitKey(1);
		if (c == 27) break;
	}
	cap.release();
	destroyAllWindows();
}

void BlackBox::bgr2Gray(Mat &_frame)
{
	cvtColor(_frame, _frame, COLOR_BGR2GRAY);
	//Canny(_frame, _frame, 100, 200);
	cvtColor(_frame, _frame, COLOR_GRAY2BGR);
	
}