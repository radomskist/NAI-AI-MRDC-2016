#include "Sensors/img_rgb.h"

imgrgb::imgrgb() {
	krgb.flags = KRGB;
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
	rgbin = img(cv::Rect(190, 0, 1539, 1080));
	cv::resize(rgbin,img, cv::Size(512,380));
	img = img + cv::Scalar(-110,-110,-110);
	cv::cvtColor(img,img,CV_BGRA2GRAY,1);
	cv::flip(img, rgbin, 1);
    cv::threshold(rgbin,rgbin, 80.0, 180.0, cv::THRESH_BINARY);
	//cv::bilateralFilter(rgbin, img, 4, 300, 300);
	cv::Mat cannystuff(512,400,CV_8UC1);
	//cv::morphologyEx(rgbin, rgbin, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(30, 30)));
	cv::morphologyEx(rgbin, img, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(30, 30)));
	//cv::dilate(img, img, cv::Mat(), cv::Point(0,0),8);

	//cv::Canny(rgbin, cannystuff, 80, 80, 3); //Detecting edges
	std::vector<cv::Vec3f> circlelist;
	//cv::HoughCircles(img, circlelist, CV_HOUGH_GRADIENT, 1, 40,100,100,0,40);
	cv::Mat matcircle = cv::Mat::zeros(400,512,CV_8UC3);
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(img,contours,hierarchy,cv::CHAIN_APPROX_NONE,cv::RETR_LIST);

	for(int i = 0; i < contours.size(); i++) {
		if(cv::pointPolygonTest(contours[i], cv::Point(256,300), false)  < 1) {
			contours.erase(contours.begin() + i);
			i--;
		}
		
	}
	
	for(int i = 0; i < contours.size(); i++)
		cv::drawContours(matcircle, contours, -1,cv::Scalar(255,255,255), -1);
	//	cv::circle(matcircle,cv::Point(circlelist[i][0],circlelist[i][1]),circlelist[i][2],cv::Scalar(255,0,0));

	int resolution = krgb.width*400;
	for(int i = 0; i < resolution; i++) {
		krgb.data[i*4] = img.data[i];
		krgb.data[i*4 + 1] = img.data[i];
		krgb.data[i*4 + 2] = img.data[i];

		//if(cannystuff.data[i])
		//	krgb.data[i*4 + 2] = cannystuff.data[i];	

		krgb.data[i*4 + 3] = 0;	
		if(matcircle.data[3*i])
			krgb.data[i*4] = matcircle.data[i*3];
	}
	//cannystuff.release();
	img.release();
	rgbin.release();
	matcircle.release();
}

nimg *imgrgb::GetImg() {
	return &krgb;

}

