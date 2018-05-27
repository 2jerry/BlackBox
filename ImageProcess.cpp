#include "ImageProcess.hpp"

ImageProcess::ImageProcess(Mat& _frame, Size _videoSize)
{
	imgSize = _videoSize;
}


// 이미지 처리의 구간 나누기 (ROI(Region Of Interesting) 설정)
void ImageProcess::ROI2Binary(Mat &_image, Point _xy, Size _size, int _mode)
{
	Mat imageROI = _image(Rect(_xy, _size)); // ROI
	if (_mode == 0)
		threshold(imageROI, imageROI, 0, 0, THRESH_BINARY);
	else
		adaptiveThreshold(imageROI, imageROI, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 9.0);

}

void ImageProcess::startProcessing(Mat& _frame)
{
	// 영상을 그레이

	ROI2Binary(_frame, Point(0, imgSize.height / 2.2), Size(imgSize.width, imgSize.height / 2.2), 1); // 관심영역 설정 (도로)
	ROI2Binary(_frame, Point(0, 0), Size(imgSize.width, imgSize.height / 2.2), 0);  // 관심영역 설정 (하늘)
	ROI2Binary(_frame, Point(0, 0), Size(imgSize.width / 2.8, imgSize.height), 0);  // 관심영역 설정 (하늘)
	ROI2Binary(_frame, Point(0, imgSize.height / 1.3), Size(imgSize.width, imgSize.height - imgSize.height / 1.3), 0); // 관심영역 설정 (자동차)

	Mat morphImg;
	//morphologyEx(_frame, _frame, MORPH_TOPHAT, Mat(9, 9, CV_8U));
	morphologyEx(_frame, _frame, MORPH_BLACKHAT, Mat(9, 9, CV_8U));
}