#include "BlackBox.hpp"

/* ���� ���
 * 1. ���� load
 * 2. �̹��� ó���� ���� ������ (ROI(Region Of Interesting) ����)
 * 3. contour ����
 * 4. ��ȣ�� ��ǥ ����
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


// 3. contour ����(�簢����)
void findRectangle()
{
	vector<vector<Point>> contours; // ������ �κ��� ���� �迭
	vector<Vec4i> hierarchy; // ������ ������ȣ
}

// 4. ��ȣ�� ��ǥ ����
// ������ 0�� ������, 1�� ����
Size* BlackBox::findRectPoint(vector<Point> _poly)
{
	Size point[2];
	int s = INT_MAX;
	int b = -1;
	for (int idx = 0; idx < 4; idx++)
	{
		if (_poly[idx].x + _poly[idx].y < s)
		{
			s = _poly[idx].x + _poly[idx].y;
			point[0] = _poly[idx];
		}
		else if (_poly[idx].x + _poly[idx].y > b)
		{
			b = _poly[idx].x + _poly[idx].y;
			point[1] = _poly[idx];
		}
	}
	return point;
}

// 5. ��ȣ���� ���� ����
/* 1) �簢���� ������ ���� ����
 * 2) ������ȣ�ǿ� �´� ���� ����
 * 3) ��ȣ���� ���̴� 0�� �� �� ���� */
bool BlackBox::numberDetect(Size* _point)
{
	int height = _point[1].height - _point[0].height;
	int width = _point[1].width - _point[0].width;
	//bool flag = true;

	if (height * width > 1000) // 1�� ����
		return false;
	if (height == 0) // 3�� ����
		return false;
	if ((width / height < 3) || (width / height >6)) // 2�� ����
		return false;

	return true;
}


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

