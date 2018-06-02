#include "FindLicencePlates.hpp"

// ���� ���
/* 1. ���� load
* 2. �簢�� �׸���(���ϴ� ��ǥ, ũ��)
* 3. ��ȣ���� ���� ����
* 4. ��ȣ�� ��ǥ ����
* 5. contour ����*/

// 1. ���� load 
/* _fname : ���� �̸�
* _format : ���� ���� (VIDEO or IMAGE) */
FindLicencePlates::FindLicencePlates(string _fname)
{
	Mat temp;

	VideoCapture _cap(_fname);
	cap = _cap;

	if (!_cap.isOpened())
	{
		cout << "Error opening video stream or file" << endl;
		return;
	}

	_cap >> frame;
	videoSize = frame.size();
	cout << "width : " << videoSize.width << ", height : " << videoSize.height << endl;


	p = new ImageProcess(videoSize); // frame�� �׷���
}

// 2. �簢�� �׸���(���ϴ� ��ǥ, ũ��) 
/* _frame :  �簢���� �׸� ����
* _xy    : �簢���� �׸� ���� ��ǥ
* _size  : �簢���� ���� ���� ���� */
void FindLicencePlates::drawRect(Mat& _frame, Point _xy, Size _size)
{
	rectangle(_frame, Rect(_xy.x, _xy.y, \
		_size.height, _size.width), RED, 2);
}

// 3. ��ȣ���� ���� ����
/* 1) �簢���� ������ ���� ����
*  2) ������ȣ�ǿ� �´� ���� ����
*  3) ��ȣ���� ���̴� 0�� �� �� ����
*  _point : �簢���� ����,�� ���� ��ǥ */
bool FindLicencePlates::numberDetect(Size* _point)
{
	int height = _point[1].height - _point[0].height;
	int width = _point[1].width - _point[0].width;
	//bool flag = true;

	if (height * width > 1000) // 1�� ����
		return false;
	if (height == 0) // 3�� ����
		return false;
	if ((width / height < 3) || (width / height > 6)) // 2�� ����
		return false;

	return true;
}

// 4. ��ȣ�� ��ǥ ���� => �簢���� �������� ������ ã���ش�.
/* point 0�� ������, 1�� ����
* _poly : ������ 4�� �������� ��ǥ ���� */
Size* FindLicencePlates::findRectPoint(vector<Point> _poly)
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

/* 5. contour ���� (����� ã��, ��ȣ�� Ȯ���� �簢���� �׸���)
* _frame : ����� ã�� ���� */
void FindLicencePlates::detectRectangle(Mat& _frame)
{
	vector<vector<Point>> contours; // ������ �κ��� ���� �迭
	vector<Vec4i> hierarchy; // ������ ������ȣ

	findContours(_frame, contours, hierarchy, \
		CV_RETR_TREE, CV_CHAIN_APPROX_NONE);

	vector<Point> poly;
	for (int i = 0; i < contours.size(); i++)
	{
		if (contours[i].size() < 100) continue;
		approxPolyDP(contours[i], poly, 7, true);
		if (poly.size() == 4)
		{
			Size* point = new Size[2];
			point = findRectPoint(poly); // �簢���� �� �� ��ǥ

			if (numberDetect(point)) // ��ȣ�ǿ� �´� �����̸� �簢�� ǥ��
				drawRect(frame, point[0], Size(point[1].height - point[0].height, point[1].width - point[0].width));
			// �簢���� ũ�⿡ �°� �׸���.
		}
	}
	imshow("Frame", frame); // �̹��� ���
}

void FindLicencePlates::playVideo(Mat& temp_frame, int _mode)
{
	cap >> frame;
	if (frame.empty())
	{
		PLAY = 0;
		return;
	}
	if(_mode == GRAY)
		cvtColor(frame, frame, COLOR_BGR2GRAY);

	temp_frame = frame.clone(); // �̹���ó���� frame�� ����
	p->startProcessing(temp_frame, MORPH_TOPHAT); // �̹��� ó�� ����

	if (_mode == GRAY)
		cvtColor(frame, frame, COLOR_GRAY2BGR);

	detectRectangle(temp_frame); // �簢�� �׸���.
	
}