//#include <opencv2\opencv.hpp>
//#include <iostream>
//
//using namespace std;
//using namespace cv;
//
//int main(int argc, char *argv[])
//{
//	Mat img_gray, img_canny;
//	Mat inputImg;
//	inputImg = imread(argv[1], IMREAD_UNCHANGED);
//	cvtColor(inputImg, img_gray, COLOR_BGR2GRAY);
//	blur(img_gray, img_canny, Size(3, 3));
//	Canny(img_canny, img_canny, 100, 200);
//
//	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
//	Mat morph;
//	morphologyEx(img_canny, morph, CV_MOP_CLOSE, kernel);
//
//	size_t idx, i;
//	vector<vector<Point>> contours;
//	vector<Vec4i> hierarchy;
//	findContours(morph, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, \
//		Point(0, 0));
//
//	Mat contours_img;
//	cvtColor(img_canny, contours_img, CV_GRAY2BGR);
//
//
//
//	vector<Point> poly;
//	Mat poly_img,rectangle_img;
//
//	cvtColor(img_canny, poly_img, CV_GRAY2BGR);
//	cvtColor(img_canny, rectangle_img, CV_GRAY2BGR);
//	//	
//
//	for (idx = 0; idx < contours.size(); idx++)
//	{
//		approxPolyDP(contours[idx], poly,5, true);
//		for (i = 0; i < poly.size(); i++)
//		{
//			if(poly.size() == 4)
//				line(poly_img, poly[i], poly[(i + 1) % poly.size()], Scalar(255, 0, 0), 2);
//		}
//		
//	}
//
//
//	imshow("re", poly_img);
//
//
//
//	waitKey(0);
//}
//
////VideoCapture cap(argv[1]);
////
////if (!cap.isOpened())
////{
////	cout << "Error opening video stream or file" << endl;
////	return -1;
////}
//
//
//
////Mat frame;
////
////cap >> frame;
////
////int wid = frame.size().width;
////int hei = frame.size().height;
////
////cout << "width : " << wid << ", height : " << hei << endl;
////while (1)
////{
////
////	cap >> frame;
////	if (frame.empty()) break;
////
////	vector<Mat> bgr_images(3);
////	split(frame, bgr_images);
////	bgr_images[0] = bgr_images[2].clone();
////	bgr_images[1] = bgr_images[2].clone();
////	merge(bgr_images, frame);
////
////	rectangle(frame, Rect(wid / 2, hei / 2, 30, 30), Scalar(0, 0, 255), 3); // 사각형 그리기
////																			cvtColor(frame, frame, COLOR_BGR2GRAY);
////
////	imshow("Frame", frame);
////	char c = (char)waitKey(1);
////	if (c == 27) break;
////}
////
////cap.release();
////destroyAllWindows();
////return 0;