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
	Mat gray_img;
	Mat imageROI, imageROI2;
	while (1)
	{
		cap >> frame;
		if (frame.empty())	break;

		//if (!_mode) // gray 일때만
		//bgr2Gray(frame);
		processedImg();
		//cvtColor(frame, gray_img, COLOR_BGR2GRAY);

		//imageROI = gray_img(Rect(0, videoSize.height / 2.2, videoSize.width, videoSize.height / 2.2));
		//imageROI2 = gray_img(Rect(0, 0, videoSize.width, videoSize.height / 2.2)); // 관심영역 설정 (하늘)
		//threshold(imageROI2, imageROI2, 0, 0, THRESH_BINARY);
		//threshold(imageROI, imageROI, 120, 255, THRESH_BINARY); // 하늘 밑에만 binary로 설정함

		//cvtColor(gray_img, gray_img, COLOR_GRAY2BGR);

		//line(gray_img, Point(0, videoSize.height / 2.2), Point(videoSize.width, videoSize.height / 2.2), \
		//	Scalar(0, 255, 0), 3);
		////drawRect(frame, videoSize/2, Size(30, 30));// 사각형 
		////drawRect(frame, Size(30, 30));// 사각형 

		//imshow("Frame", gray_img);

		char c = (char)waitKey(1);
		if (c == 27) break;
	}
	cap.release();
	destroyAllWindows();
}

void BlackBox::processedImg()
{
	/*---------- Image Processing -----------*/
	//bgr2Gray(frame);

	//morphologyEx(frame, frame, MORPH_BLACKHAT, kernel);
	Mat grayImg;
	cvtColor(frame, grayImg, COLOR_BGR2GRAY);
	cvtColor(frame, frame, COLOR_BGR2GRAY);
	Mat blurImg;
	blur(grayImg, blurImg, Size(2, 2)); // 윤곽
	//Canny(blurImg, blurImg, 100, 200);
	Mat imageROI, imageROI2, imageROI3;
	imageROI = blurImg(Rect(0, videoSize.height / 2.2, videoSize.width, videoSize.height / 2.2)); // 관심영역 설정 (하늘밑에)
	imageROI2 = blurImg(Rect(0, 0, videoSize.width, videoSize.height / 2.2)); // 관심영역 설정 (하늘)
	imageROI3 = blurImg(Rect(0, videoSize.height / 1.3, videoSize.width, videoSize.height - videoSize.height / 1.3));

	threshold(imageROI3, imageROI3, 0, 0, THRESH_BINARY);
	threshold(imageROI2, imageROI2, 0, 0, THRESH_BINARY);
	//threshold(imageROI, imageROI, 120, 255, THRESH_BINARY); // 하늘 밑에만 binary로 설정함
	adaptiveThreshold(imageROI, imageROI, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY,11, 9.0);


	Mat morphImg;
	Mat kernel(9, 9, CV_8U); // 커널값 클수록 흰 영역 많아짐.
	morphologyEx(blurImg, morphImg, MORPH_TOPHAT, kernel);

	Mat temp = morphImg.clone();
	imshow("morphImg", temp);


	/*---------- Find Contour ----------*/
	vector<vector<Point>> contours; // 윤곽선 부분을 넣을 배열
	vector<Vec4i> hierarchy; // 윤곽선 고유번호

	findContours(temp, contours, hierarchy, \
		CV_RETR_TREE, CV_CHAIN_APPROX_NONE);

	//cout << contours.size() << endl;
	cvtColor(frame, frame, COLOR_GRAY2BGR);
	cvtColor(temp, temp, COLOR_GRAY2BGR);

	//for (int i = 1; i < contours.size(); i++)
	//{
	//	if (contours[i].size() > 100 && contours[i].size()<200)
	//		drawContours(frame, contours, i, Scalar(0, 0, 255), 1, 4, \
		//			hierarchy);
//}

	vector<Point> poly;
	for (int i = 0; i < contours.size(); i++)
	{
		if (contours[i].size() < 100) continue;
		//if (contourArea(contours[i]) < 300 || contourArea(contours[i]) > 1500) continue;
		approxPolyDP(contours[i], poly, 7, true);
			if (poly.size() == 4)
			{
				/*drawContours(frame, contours, i, Scalar(0, 0, 255), 2, 4, \
				hierarchy);*/

				// 원점이랑 제일 가까운애 찾기
				int s = poly[0].x + poly[0].y;
				int b = -1; Size bb;
				Size ss = poly[0];
				for (int idx = 1; idx < 4; idx++)
				{
					if (poly[idx].x + poly[idx].y < s)
					{
						s = poly[idx].x + poly[idx].y;
						ss = poly[idx];
					}
					else if (poly[idx].x + poly[idx].y > b)
					{
						b = poly[idx].x + poly[idx].y;
						bb = poly[idx];
					}
				}
				if ((bb.height - ss.height)* (bb.width - ss.width) > 1000) continue;
				//if ((bb.width - ss.width) / (bb.height - ss.height) < 3)continue;
				if ((bb.height - ss.height) == 0)continue;
				if ((bb.width - ss.width) / (bb.height - ss.height) > 3 &&\
					(bb.width - ss.width) / (bb.height - ss.height)<8)
				{
					drawRect(frame, ss, Size(bb.height - ss.height, bb.width - ss.width));
					cout << "가로 세로 비율 " << (bb.width - ss.width) / (bb.height - ss.height) << endl;
				}
				
				//cout << i << ": " << contourArea(contours[i]) << endl;
				imshow("Frame", frame);
				//waitKey(0);
			
		
		}
	}




	///*---------- Show Image ----------*/
	//cvtColor(morphImg, morphImg, COLOR_GRAY2BGR);

	//line(frame, Point(0, videoSize.height / 2.2), Point(videoSize.width, videoSize.height / 2.2), \
	//	Scalar(0, 255, 0), 3);

	//line(frame, Point(0, videoSize.height / 1.3), Point(videoSize.width, videoSize.height / 1.3), \
	//	Scalar(0, 255, 0), 3);



	//waitKey(0);
}

void BlackBox::bgr2Gray(Mat &_frame)
{
	cvtColor(_frame, _frame, COLOR_BGR2GRAY);
	//Canny(_frame, _frame,300,200);
	adaptiveThreshold(_frame, _frame, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 5, 11.0);
	//Canny(_frame, _frame, 100, 200);
	cvtColor(_frame, _frame, COLOR_GRAY2BGR);

}