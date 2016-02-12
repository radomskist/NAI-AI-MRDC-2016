#include "Sensors/img_rgb.h"

imgrgb::imgrgb() {
	krgb.flags = KRGB;
	krgb.width = 512;
	krgb.height = 424;
	krgb.depth = 4;
	krgb.data = new unsigned char[krgb.width*krgb.height*krgb.depth];
	groundmat = cv::Mat::zeros(380,512,CV_8UC1);


	cv::SimpleBlobDetector::Params ballparam;
	ballparam.minDistBetweenBlobs = 0.0f;
	ballparam.filterByInertia = true;
	ballparam.filterByConvexity = false;
	ballparam.filterByColor = false;
	ballparam.filterByCircularity = true;
	ballparam.filterByArea = true;
	ballparam.maxCircularity = 1.0;
	ballparam.minCircularity = .8;
	ballparam.maxInertiaRatio = 1.0;
	ballparam.minInertiaRatio = .6;
	ballparam.minArea = 1.0f;
	ballparam.maxArea = 500.0f;
	
	balldet = cv::SimpleBlobDetector::create(ballparam); 
}

imgrgb::~imgrgb() {

}

void imgrgb::findground(cv::Mat &hsvin) {
	cv::Mat img3;
	cv::Mat img2;
	groundmat.release();
	groundmat = cv::Mat::zeros(380,512,CV_8UC1);

	cv::threshold(hsvin,img2, 200.0, 210.0, cv::THRESH_BINARY);
	cv::morphologyEx(img2, img3, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(30, 30)));
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(img3,contours,hierarchy,cv::CHAIN_APPROX_NONE,cv::RETR_LIST);
	
	for(int i = 0; i < contours.size(); i++) {
		int testpoints = 0;
		if (cv::pointPolygonTest(contours[i], cv::Point(102,300), false) >= 0) 
			testpoints++;
		if (cv::pointPolygonTest(contours[i], cv::Point(204,300), false) >= 0) 
			testpoints++;
		if (cv::pointPolygonTest(contours[i], cv::Point(256,300), false) >= 0) 
			testpoints++;
		if (cv::pointPolygonTest(contours[i], cv::Point(306,300), false) >= 0) 
			testpoints++;
		if (cv::pointPolygonTest(contours[i], cv::Point(408,300), false) >= 0) 
			testpoints++;

		if(testpoints < 2) {
			contours.erase(contours.begin() + i);
			i--;
		}	
	}

	for(int i = 0; i < contours.size(); i++)
		cv::drawContours(groundmat, contours, -1,cv::Scalar(255,255,255), -1);

	img2.release();
	img3.release();
}

void imgrgb::findballs(cv::Mat &hsvin, cv::Mat &circlemat) {
	circlemat.release();
	circlemat = cv::Mat::zeros(380,512,CV_8UC1);
	
	//std::vector<cv::Vec3f> circlelist;
	cv::Mat closed;

	//cv::Canny(hsvin, canny, 80, 80, 3, false);
	cv::morphologyEx(hsvin, closed, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(25, 25)));
	//cv::morphologyEx(canny, hsvin, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10, 10)));

	//cv::Canny(hsvin, canny, 80, 80, 3, false);
	std::vector<cv::KeyPoint> circlelist;
	balldet->detect(closed,circlelist);

	//cv::HoughCircles(canny, circlelist, CV_HOUGH_GRADIENT, 1, 200, 1,15, 2, 30);
	//for(int i = 0; i < circlelist.size(); i++)
	//	circle(circlemat, cv::Point(circlelist[i][0],circlelist[i][1]), circlelist[i][2], cv::Scalar(255,255,255),CV_FILLED, 8,0);

	for(int i = 0; i < circlelist.size(); i++) {
		if(circlelist[i].pt.y < 120) {
			circlelist.erase(circlelist.begin() + i);
			i--;
			continue;
		}

		circle(circlemat, cv::Point(circlelist[i].pt.x,circlelist[i].pt.y), 10, cv::Scalar(255,255,255),CV_FILLED, 8,0);
	}

	//cv::circle(matcircle,cv::Point(circlelist[i][0],circlelist[i][1]),circlelist[i][2],cv::Scalar(255,0,0));
	/*cv::threshold(hsvin,img2, 200.0, 245.0, cv::THRESH_BINARY);
	cv::morphologyEx(img2, img3, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(30, 30)));
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(img3,contours,hierarchy,cv::CHAIN_APPROX_NONE,cv::RETR_LIST);
*/
}

void imgrgb::ProcessImg(unsigned char *rgbbuff) {
	//Image needs to be scaled down
	//1920 -> 512, 1080 -> 424
	//3.75x, 2.47x
	cv::Mat img(1080,1920, CV_8UC4, rgbbuff);
	cv::Mat rgbin;
	cv::Mat HSVin;
	//cv::cvtColor(rgbin,img,CV_BGR2HSV);
	rgbin = img(cv::Rect(190, 0, 1539, 1080));
	cv::resize(rgbin,img, cv::Size(512,380));
	cv::flip(img, rgbin, 1);
	img.release();
	cv::cvtColor(rgbin,HSVin,CV_BGR2HSV);
	cv::Mat channels[3];
	cv::split(HSVin, channels);

	findground(channels[2]);
	cv::Mat circlesstuff;
	findballs(channels[1], circlesstuff);

	int resolution = krgb.width*380;
	for(int i = 0; i < resolution; i++) {

		/*
		krgb.data[i*4] = rgbin.data[i*4];
		krgb.data[i*4 + 1] = rgbin.data[i*4+1];
		krgb.data[i*4 + 2] = rgbin.data[i*4+2];*/
		krgb.data[i*4] = rgbin.data[i*4];
		krgb.data[i*4 + 1] = rgbin.data[i*4 + 1];
		krgb.data[i*4 + 2] = rgbin.data[i*4 + 2];
		//if(cannystuff.data[i])
		//	krgb.data[i*4 + 2] = cannystuff.data[i];	

		//krgb.data[i*4 + 3] = 0;	
		if(circlesstuff.data[i]) {
			krgb.data[i*4] = 0;
			krgb.data[i*4 + 1] = circlesstuff.data[i];
			krgb.data[i*4 + 2] = 0;
		}
		else if(groundmat.data[i]) {
			krgb.data[i*4] = groundmat.data[i];
			krgb.data[i*4 + 1] = 0;
			krgb.data[i*4 + 2] = 0;
		}
	}
	//cannystuff.release();
	img.release();
	rgbin.release();
	channels[0].release();
	channels[1].release();
	channels[2].release();
	//matcircle.release();
}

nimg *imgrgb::GetImg() {
	return &krgb;

}

