#include "BlackBox.hpp"

/* 구현 목록
 * 1. 파일 load
 * 2. 이미지 처리의 구간 나누기 (ROI(Region Of Interesting) 설정)
 * 3. contour 추출을 위해 이미지를 binary로 설정
 * 4. contour 추출
 * 5. 번호판의 조건 설정
*/
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

BlackBox::~BlackBox()
{
}

void BlackBox::drawRect(Mat& _frame, Size _xy, Size _size)
{
	rectangle(_frame, Rect(_xy.width, _xy.height, \
		_size.height, _size.width), Scalar(0, 0, 255), 2);

}

// 2. 이미지 처리의 구간 나누기(ROI 설정)
void BlackBox::ROI2Threshold(Mat &_image, Point _xy, Size _size, int _mode)
{
	Mat imageROI = _image(Rect(_xy, _size)); // ROI
	if (_mode == 0)
		threshold(imageROI, imageROI, 0, 0, THRESH_BINARY);
	else
		adaptiveThreshold(imageROI, imageROI, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 9.0);
}
// 3. contour 추출을 위해 이미지를 binary로 설정
Mat& BlackBox::getMorphologyImage(Mat _image, Mat _kernel, int _mode)
{
	//Mat morphImg;
	morphologyEx(_image, proccessImg, _mode, _kernel);
	return proccessImg;
}
// 4. contour 추출
// 5. 번호판의 조건 설정

void BlackBox::processedVideo(int _mode)
{
	Mat gray_img;
	Mat imageROI, imageROI2;
	while (1)
	{
		cap >> frame;
		if (frame.empty())	break;

		//if (!_mode) // gray 일때만
		//bgr2Gray(frame);
		processedImg();
	

		char c = (char)waitKey(1);
		if (c == 27) break;
	}
	cap.release();
	destroyAllWindows();
}




void BlackBox::bgr2Gray(Mat &_frame)
{
	cvtColor(_frame, _frame, COLOR_BGR2GRAY);
	//Canny(_frame, _frame,300,200);
	adaptiveThreshold(_frame, _frame, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 5, 11.0);
	//Canny(_frame, _frame, 100, 200);
	cvtColor(_frame, _frame, COLOR_GRAY2BGR);

}