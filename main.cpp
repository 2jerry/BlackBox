#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
	VideoCapture cap(argv[1]);

	if (!cap.isOpened())
	{
		cout << "Error opening video stream or file" << endl;
		return -1;
	}



	Mat frame;
	
	cap >> frame;
	
	int wid = frame.size().width;
	int hei = frame.size().height;

	cout << "width : " << wid << ", height : " << hei << endl;
	while (1)
	{
		
		cap >> frame;
		if (frame.empty()) break;
		
		vector<Mat> bgr_images(3);
		split(frame, bgr_images);
		bgr_images[0] = bgr_images[2].clone();
		bgr_images[1] = bgr_images[2].clone();
		merge(bgr_images, frame);

		rectangle(frame, Rect(wid / 2, hei / 2, 30, 30), Scalar(0, 0, 255), 3); // 사각형 그리기
		//cvtColor(frame, frame, COLOR_BGR2GRAY);

		imshow("Frame", frame);
		char c = (char)waitKey(1);
		if (c == 27) break;
	}
	
	cap.release();
	destroyAllWindows();
	return 0;

}