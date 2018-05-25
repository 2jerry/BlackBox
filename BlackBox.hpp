#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
#include<algorithm>

using namespace std;
using namespace cv;

#define COLOR 1
#define GRAY 0
#define VIDEO 2
#define IMAGE 3
class BlackBox
{
private:
	Mat frame;
	Mat proccessImg;
	VideoCapture cap;
	Size videoSize;

public:
	BlackBox(string fname,int _format);
	BlackBox()	{	}// 기본 생성자
	~BlackBox();

	void ROI2Threshold(Mat &_image, Point _xy, Size _size, int _mode);
	void drawRect(Mat& _frame, Size _xy,Size _size);
	void processedVideo(int _mode);
	Mat& getMorphologyImage(Mat _image, Mat _kernel, int _mode);
	void processedImg()
	{
		/*---------- Image Processing -----------*/
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		Mat blurImg;
		blur(frame, blurImg, Size(2, 2)); // 윤곽
		
		ROI2Threshold(blurImg, Point(0, videoSize.height / 2.2), Size(videoSize.width, videoSize.height / 2.2),1); // 관심영역 설정 (도로)
		ROI2Threshold(blurImg, Point(0, 0), Size(videoSize.width, videoSize.height / 2.2), 0);  // 관심영역 설정 (하늘)
		ROI2Threshold(blurImg, Point(0, videoSize.height / 1.3), Size(videoSize.width, videoSize.height - videoSize.height / 1.3), 0); // 관심영역 설정 (자동차)
		
		
		Mat morphImg =  getMorphologyImage(blurImg, Mat(9, 9, CV_8U), MORPH_TOPHAT);
		 // 커널값 클수록 흰 영역 많아짐.
		//morphologyEx(blurImg, morphImg,MORPH_TOPHAT, kernel);

		////Mat temp = morphImg.clone(); // 새로운 공간 생성
		//imshow("morphImg", morphImg);
		
		imshow("blurImg", blurImg);
		imshow("morphImg", morphImg);


		//*---------- Find Contour ----------*/
		vector<vector<Point>> contours; // 윤곽선 부분을 넣을 배열
		vector<Vec4i> hierarchy; // 윤곽선 고유번호

		findContours(morphImg, contours, hierarchy, \
			CV_RETR_TREE, CV_CHAIN_APPROX_NONE);

		//cout << contours.size() << endl;
		cvtColor(frame, frame, COLOR_GRAY2BGR);
		cvtColor(morphImg, morphImg, COLOR_GRAY2BGR);

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
				if ((bb.width - ss.width) / (bb.height - ss.height) > 3 && \
					(bb.width - ss.width) / (bb.height - ss.height)<8)
				{
					drawRect(frame, ss, Size(bb.height - ss.height, bb.width - ss.width));
					cout << "가로 세로 비율 " << (bb.width - ss.width) / (bb.height - ss.height) << endl;
				}

				//cout << i << ": " << contourArea(contours[i]) << endl;
				imshow("Frame", frame);
				waitKey(0);


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
	void bgr2Gray(Mat &_frame);
	
};

