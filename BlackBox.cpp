#include "BlackBox.hpp"

/* 구현 목록
 * 1. 파일 load
 * 2. 이미지 처리의 구간 나누기 (ROI(Region Of Interesting) 설정)
 * 3. contour 추출
 * 4. 번호판 좌표 추출
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


// 3. contour 추출(사각형만)
void findRectangle()
{
	vector<vector<Point>> contours; // 윤곽선 부분을 넣을 배열
	vector<Vec4i> hierarchy; // 윤곽선 고유번호
}

// 4. 번호판 좌표 추출
// 백터의 0은 시작점, 1은 끝점
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

// 5. 번호판의 조건 설정
/* 1) 사각형의 면적의 조건 설정
 * 2) 차량번호판에 맞는 비율 설정
 * 3) 번호판의 높이는 0이 될 수 없음 */
bool BlackBox::numberDetect(Size* _point)
{
	int height = _point[1].height - _point[0].height;
	int width = _point[1].width - _point[0].width;
	//bool flag = true;

	if (height * width > 1000) // 1번 조건
		return false;
	if (height == 0) // 3번 조건
		return false;
	if ((width / height < 3) || (width / height >6)) // 2번 조건
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

		//if (!_mode) // gray 일때만
		//bgr2Gray(frame);
		processedImg();


		char c = (char)waitKey(1);
		if (c == 27) break;
	}
	cap.release();
	destroyAllWindows();
}

