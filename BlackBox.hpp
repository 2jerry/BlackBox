#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
#include<algorithm>

using namespace std;
using namespace cv;

#define GRAY 0
#define COLOR 1
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
	BlackBox()	{	}// �⺻ ������
	~BlackBox();

	void ROI2Threshold(Mat &_image, Point _xy, Size _size, int _mode);
	void drawRect(Mat& _frame, Size _xy,Size _size);
	void processedVideo(int _mode);
	bool numberDetect(Size* _point);
	Size* findRectPoint(vector<Point> _poly);
	void processedImg()
	{
		/*---------- Image Procepoint[0]ing -----------*/
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		Mat blurImg;
		blur(frame, blurImg, Size(2, 2)); // ����
		
		ROI2Threshold(blurImg, Point(0, videoSize.height / 2.2), Size(videoSize.width, videoSize.height / 2.2),1); // ���ɿ��� ���� (����)
		ROI2Threshold(blurImg, Point(0, 0), Size(videoSize.width, videoSize.height / 2.2), 0);  // ���ɿ��� ���� (�ϴ�)
		ROI2Threshold(blurImg, Point(0, videoSize.height / 1.3), Size(videoSize.width, videoSize.height - videoSize.height / 1.3), 0); // ���ɿ��� ���� (�ڵ���)
		
		// Ŀ�ΰ� Ŭ���� �� ���� ������.																															   // Ŀ�ΰ� Ŭ���� �� ���� ������.
		Mat morphImg;
		morphologyEx(blurImg, morphImg, MORPH_TOPHAT, Mat(9, 9, CV_8U));
				
		imshow("blurImg", blurImg);
		imshow("morphImg", morphImg);


		//*---------- Find Contour ----------*/
		vector<vector<Point>> contours; // ������ �κ��� ���� �迭
		vector<Vec4i> hierarchy; // ������ ������ȣ

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
				// �����̶� ���� ������ ã��
				Size* point = new Size[2];
				point = findRectPoint(poly);
				
				if(numberDetect(point))
				{
					drawRect(frame, point[0], Size(point[1].height - point[0].height, point[1].width - point[0].width));
					//cout << "���� ���� ���� " << (point[1].width - point[0].width) / (point[1].height - point[0].height) << endl;
				}

				imshow("Frame", frame);
				//waitKey(0);

			}
		}

	}
	
	
};

