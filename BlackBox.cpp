#include "BlackBox.hpp"

BlackBox::BlackBox(string fname, int _format)
{
	if (_format == VIDEO)
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
	else
	{

		frame = imread(fname, IMREAD_UNCHANGED);
		videoSize = frame.size();
	}

}

BlackBox::BlackBox()
{
}

BlackBox::~BlackBox()
{
}

void BlackBox::drawRect(Mat& _frame, Size _xy, Size _size)
{
	rectangle(_frame, Rect(_xy.width, _xy.height, \
		_size.height, _size.width), Scalar(0, 0, 255), 2);
}

void BlackBox::processedVideo(int _mode)
{
	while (1)
	{

		cap >> frame;
		//resize(frame, frame, Size(), 0.5, 0.5);
		if (frame.empty())	break;

		if (!_mode) // gray 일때만
			bgr2Gray(frame);


		Mat _ff;

		Mat kernel(7, 7, CV_8U);
		morphologyEx(frame, frame, MORPH_BLACKHAT, kernel);
		//morphologyEx(frame, frame, MORPH_TOPHAT, kernel);

		line(frame, Point(0, videoSize.height / 2.2), Point(videoSize.width, videoSize.height / 2.2), \
			Scalar(0, 255, 0), 3); // LIMIT line


		//vector<vector<Point>> contours;
		//findContours(frame,contours, CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
		//Mat result(frame.size(), CV_8U, Scalar(255));		
		//drawContours(result, contours, -1, Scalar(0, 0, 255), 2);

		// limit line 

		for (int i = videoSize.height / 2.2; i < videoSize.height; i++)
		{
			for (int j = 0; j < videoSize.width / 2.2; j++)
			{

			}
		}


		//for (int i = 0; i < videoSize.width; i++)
		//{
		//	for (int j = 0; j < videoSize.height / 2.2; j++)
		//	{
		//		
		//		//frame.
		//		//bool white_pixel = (B >= 200 && G >= 200 && R >= 200);
		//		//if (white_pixel == false)
		//		//{
		//		//	//drawRect(_ff,Size(j,i), Size(10, 10));// 사각형 
		//		//}

		//	}
		//}

		//drawRect(frame, videoSize/2, Size(30, 30));// 사각형 
		//drawRect(frame, Size(30, 30));// 사각형 

		imshow("Frame", frame);

		char c = (char)waitKey(1);
		if (c == 27) break;
	}
	cap.release();
	destroyAllWindows();
}

void BlackBox::processedImg()
{
	//bgr2Gray(frame);
	Mat morph;
	//morphologyEx(frame, frame, MORPH_BLACKHAT, kernel);
	cvtColor(frame, frame, COLOR_BGR2GRAY);
	//blur(frame, frame, Size(2, 2)); // 윤곽

	Mat imageROI, imageROI2;
	imageROI = frame(Rect(0, videoSize.height / 2.2,videoSize.width, videoSize.height / 2.2)); // 관심영역 설정 (하늘밑에)
	imageROI2 = frame(Rect(0, 0, videoSize.width, videoSize.height / 2.2)); // 관심영역 설정 (하늘)
	threshold(imageROI2, imageROI2,0,0, THRESH_BINARY);
	threshold(imageROI, imageROI, 120,255, THRESH_BINARY); // 하늘 밑에만 binary로 설정함
	//Mat kernel(11,11, CV_8U);
	//morphologyEx(frame, morph, MORPH_BLACKHAT, kernel);
	//Canny(frame, frame, 100, 200);
	cvtColor(frame, frame, COLOR_GRAY2BGR);
	line(frame, Point(0, videoSize.height / 2.2), Point(videoSize.width, videoSize.height / 2.2), \
		Scalar(0, 255, 0), 3);
	imshow("Frame", frame);
	//imshow("imageROI", imageROI);
	waitKey(0);
}

void BlackBox::bgr2Gray(Mat &_frame)
{
	cvtColor(_frame, _frame, COLOR_BGR2GRAY);
	//Canny(_frame, _frame,300,200);
	adaptiveThreshold(_frame, _frame, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 5, 11.0);
	//Canny(_frame, _frame, 100, 200);
	cvtColor(_frame, _frame, COLOR_GRAY2BGR);

}