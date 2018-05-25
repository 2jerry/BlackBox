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
	VideoCapture cap;
	Size videoSize;

public:
	BlackBox(string fname,int _format);
	BlackBox()	{	}// 기본 생성자
	~BlackBox();

	void ROI2Threshold(Mat &_image, Point _xy, Size _size, int _mode);
	void drawRect(Mat& _frame, Size _xy,Size _size);
	void processedVideo(int _mode);
	Size* findRectPoint(vector<Point> _poly);

	void processedImg()
	{
		/*---------- Image Procepoint[0]ing -----------*/
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		Mat blurImg;
		blur(frame, blurImg, Size(2, 2)); // 윤곽
		
		ROI2Threshold(blurImg, Point(0, videoSize.height / 2.2), Size(videoSize.width, videoSize.height / 2.2),1); // 관심영역 설정 (도로)
		ROI2Threshold(blurImg, Point(0, 0), Size(videoSize.width, videoSize.height / 2.2), 0);  // 관심영역 설정 (하늘)
		ROI2Threshold(blurImg, Point(0, videoSize.height / 1.3), Size(videoSize.width, videoSize.height - videoSize.height / 1.3), 0); // 관심영역 설정 (자동차)
		
		// 커널값 클수록 흰 영역 많아짐.																															   // 커널값 클수록 흰 영역 많아짐.
		Mat morphImg;
		morphologyEx(blurImg, morphImg, MORPH_TOPHAT, Mat(9, 9, CV_8U));
				
		imshow("blurImg", blurImg);
		imshow("morphImg", morphImg);


		//*---------- Find Contour ----------*/
		vector<vector<Point>> contours; // 윤곽선 부분을 넣을 배열
		vector<Vec4i> hierarchy; // 윤곽선 고유번호

		findContours(morphImg, contours, hierarchy, \
			CV_RETR_TREE, CV_CHAIN_APPROX_NONE);

		cvtColor(frame, frame, COLOR_GRAY2BGR);
		//cvtColor(morphImg, morphImg, COLOR_GRAY2BGR);


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
				Size* point = new Size[2];
				point = findRectPoint(poly);
				
				if ((point[1].height - point[0].height)* (point[1].width - point[0].width) > 1000) continue;
				//if ((point[1].width - point[0].width) / (point[1].height - point[0].height) < 3)continue;
				if ((point[1].height - point[0].height) == 0)continue;
				if ((point[1].width - point[0].width) / (point[1].height - point[0].height) > 3 && \
					(point[1].width - point[0].width) / (point[1].height - point[0].height)<8)
				{
					drawRect(frame, point[0], Size(point[1].height - point[0].height, point[1].width - point[0].width));
					cout << "가로 세로 비율 " << (point[1].width - point[0].width) / (point[1].height - point[0].height) << endl;
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

