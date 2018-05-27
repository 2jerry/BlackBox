#include "BlackBox.hpp"

// 구현 목록
/* 1. 파일 load
* 2. 사각형 그리기(원하는 좌표, 크기)
* 3. 번호판의 조건 설정
* 4. 번호판 좌표 추출
* 5. contour 추출*/

// 1. 파일 load 
/* _fname : 파일 이름
* _format : 파일 형식 (VIDEO or IMAGE) */
BlackBox::BlackBox(string _fname, int _format)
{
	Mat temp;
	if (_format == VIDEO)
	{
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
	}
	else
	{
		frame = imread(_fname, IMREAD_UNCHANGED);
		videoSize = frame.size();
	}
	p = new ImageProcess(frame, videoSize); // frame은 그레이
}

// 2. 사각형 그리기(원하는 좌표, 크기) 
/* _frame :  사각형을 그릴 영상
* _xy    : 사각형을 그릴 시작 좌표
* _size  : 사각형의 가로 세로 길이 */
void BlackBox::drawRect(Mat& _frame, Size _xy, Size _size)
{
	rectangle(_frame, Rect(_xy.width, _xy.height, \
		_size.height, _size.width), Scalar(0, 0, 255), 2);
}

// 3. 번호판의 조건 설정
/* 1) 사각형의 면적의 조건 설정
*  2) 차량번호판에 맞는 비율 설정
*  3) 번호판의 높이는 0이 될 수 없음
*  _point : 사각형의 시작,끝 점의 좌표 */
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

// 4. 번호판 좌표 추출 => 사각형의 시작점과 끝점을 찾아준다.
/* point 0은 시작점, 1은 끝점
* _poly : 추출한 4개 꼭지점의 좌표 백터 */
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

/* 5. contour 추출 (컨투어를 찾고, 번호판 확률에 사각형을 그린다)
* _frame : 컨투어를 찾을 영상 */
void BlackBox::detectRectangle(Mat& _frame)
{
	vector<vector<Point>> contours; // 윤곽선 부분을 넣을 배열
	vector<Vec4i> hierarchy; // 윤곽선 고유번호

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
			point = findRectPoint(poly); // 사각형의 두 개 좌표

			if (numberDetect(point)) // 번호판에 맞는 조건이면 사각형 표시
				drawRect(frame, point[0], Size(point[1].height - point[0].height, point[1].width - point[0].width));
			// 사각형은 크기에 맞게 그린다.
		}
	}
	imshow("Frame", frame); // 이미지 출력
}

void BlackBox::playVideo(int _mode)
{
	Mat temp_frame;
	while (1)
	{
		cap >> frame;
		if (frame.empty())	break;

		cvtColor(frame, frame, COLOR_BGR2GRAY);
		temp_frame = frame.clone(); // 이미지처리용 frame에 저장

		p->startProcessing(temp_frame); // 이미지 처리 시작

		cvtColor(frame, frame, COLOR_GRAY2BGR);
		detectRectangle(temp_frame); // 사각형 그린다.

									 //waitKey(0);
		char c = (char)waitKey(1);
		if (c == 27) break;
	}
	delete p; // 객체 p 정리
	cap.release();
	destroyAllWindows();
}
