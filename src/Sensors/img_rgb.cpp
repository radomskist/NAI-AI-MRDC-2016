#include "Sensors/img_rgb.h"

imgrgb::imgrgb() {
	krgb.flags &= KRGB;
	krgb.width = 512;
	krgb.height = 424;
	krgb.depth = 4;
	krgb.data = new unsigned char[krgb.width*krgb.height*krgb.depth];
}

imgrgb::~imgrgb() {

}

void imgrgb::ProcessImg(unsigned char *rgbbuff) {
	//Image needs to be scaled down
	//1920 -> 512, 1080 -> 424
	//3.75x, 2.47x
	cv::Mat img(1080,1920, CV_8UC4, rgbbuff);
	cv::Mat rgbin;
	//cv::cvtColor(rgbin,img,CV_BGR2HSV);
	cv::flip(img, rgbin, 1);
	img = rgbin(cv::Rect(120, 0, 1539, 1080));
	cv::resize(img,rgbin, cv::Size(512,400));
	cv::cvtColor(rgbin,rgbin,CV_BGRA2BGR);

	cv::bilateralFilter(rgbin, img, 4, 150, 150);
	cv::Mat cannystuff(512,400,CV_8UC1);
	//cv::morphologyEx(rgbin, img, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
	cv::morphologyEx(img, img, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(20, 20)));

	cv::Canny(img, cannystuff, 80, 80, 3, false); //Detecting edges
	//std::vector<cv::Vec3f> circlelist;
	//cv::HoughCircles(cannystuff, circlelist, CV_HOUGH_GRADIENT, 3, 8);
	//cv::Mat matcircle = cv::Mat::zeros(512,400,CV_8UC1);

	//for(int i = 0; i < circlelist.size(); i++)
	//	cv::circle(matcircle,cv::Point(circlelist[i][0],circlelist[i][1]),circlelist[i][2],cv::Scalar(255,255,255),-1);

	int resolution = krgb.width*400;
	for(int i = 0; i < resolution; i++) {
		krgb.data[i*4] = img.data[i*3];
		krgb.data[i*4 + 1] = img.data[i*3 + 1];
		krgb.data[i*4 + 2] = img.data[i*3 + 2];	
		krgb.data[i*4 + 3] = 0;	
	}
	img.release();
	rgbin.release();
	cannystuff.release();
}

nimg *imgrgb::GetImg() {
	return &krgb;

}

