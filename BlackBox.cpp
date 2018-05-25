#include "BlackBox.hpp"

/* ���� ���
 * 1. ���� load
 * 2. �̹��� ó���� ���� ������ (ROI(Region Of Interesting) ����)
 * 3. contour ������ ���� �̹����� binary�� ����
 * 4. contour ����
 * 5. ��ȣ���� ���� ����
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

// 2. �̹��� ó���� ���� ������(ROI ����)
void BlackBox::ROI2Threshold(Mat &_image, Point _xy, Size _size, int _mode)
{
	Mat imageROI = _image(Rect(_xy, _size)); // ROI
	if (_mode == 0)
		threshold(imageROI, imageROI, 0, 0, THRESH_BINARY);
	else
		adaptiveThreshold(imageROI, imageROI, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 9.0);
}
// 3. contour ������ ���� �̹����� binary�� ����
Mat& BlackBox::getMorphologyImage(Mat _image, Mat _kernel, int _mode)
{
	//Mat morphImg;
	morphologyEx(_image, proccessImg, _mode, _kernel);
	return proccessImg;
}
// 4. contour ����
// 5. ��ȣ���� ���� ����

void BlackBox::processedVideo(int _mode)
{
	Mat gray_img;
	Mat imageROI, imageROI2;
	while (1)
	{
		cap >> frame;
		if (frame.empty())	break;

		//if (!_mode) // gray �϶���
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