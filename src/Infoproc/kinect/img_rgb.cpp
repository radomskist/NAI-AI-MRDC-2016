#include "Infoproc/kinect/img_rgb.h"

imgrgb::imgrgb(unsigned char set_color) {
	floory = 350; //TODO make it load from config file

	ballcolor = set_color;
	krgb.flags = KRGB;
	krgb.width = 512;
	krgb.height = 424;
	krgb.depth = 4;
	krgb.data = new unsigned char[krgb.width*krgb.height*krgb.depth];
	groundmat = cv::Mat::zeros(380,512,CV_8UC1);
	floorcolor = 0;
	floortol = 8;

	cv::SimpleBlobDetector::Params ballparam;
	ballparam.minDistBetweenBlobs = 30.0f;
	ballparam.filterByInertia = false;
	ballparam.filterByConvexity = true;
	ballparam.filterByColor = true;
	ballparam.filterByCircularity = false;
	ballparam.filterByArea = true;
	ballparam.minArea = 1.0f;
	ballparam.maxArea = 500.0f;
	ballparam.blobColor = 255;
	ballparam.minConvexity = .95;
	ballparam.maxConvexity = 1.0;

	balldet = cv::SimpleBlobDetector::create(ballparam); 
}

imgrgb::~imgrgb() {

}

void imgrgb::findground(cv::Mat &hsvin) {
	cv::Mat img3;
	cv::Mat img2;
	groundmat.release();
	groundmat = cv::Mat::zeros(380,512,CV_8UC1);

	/*getting range of values to set the floor as*/
	unsigned char low = 255;
	unsigned char high = 0;
	int ypos = 512*floory;
	for(int i = 0; i < 3; i++) {
		unsigned char set = hsvin.data[ypos + 204+102*i];
		if(set < low)
			low = set;

		if (set > high)
			high = set;
	}

	//Seeing if floor color matches
	unsigned char avg = (low + high) * .5f;
	if(floorcolor == 0)
		floorcolor = avg;
	else if(abs(avg - floorcolor) > floortol)
		return;

	if(low > 10)
		low -= floortol;
	if(high < 245)
		high += floortol;

	cv::inRange(hsvin, low, high,img2);
	cv::morphologyEx(img2, img3, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10,10)));

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(img3,contours,hierarchy,cv::CHAIN_APPROX_NONE,cv::RETR_LIST);
	
	//TODO clean this crap
	for(int i = 0; i < contours.size(); i++) {
		int testpoints = 0;
		if (cv::pointPolygonTest(contours[i], cv::Point(102,floory), false) >= 0) 
			testpoints++;
		if (cv::pointPolygonTest(contours[i], cv::Point(204,floory), false) >= 0) 
			testpoints++;
		if (cv::pointPolygonTest(contours[i], cv::Point(256,floory), false) >= 0) 
			testpoints++;
		if (cv::pointPolygonTest(contours[i], cv::Point(306,floory), false) >= 0) 
			testpoints++;
		if (cv::pointPolygonTest(contours[i], cv::Point(408,floory), false) >= 0) 
			testpoints++;

		//If full screen is blue theres problems
		//cv::contourArea(contours[i]) > 153600
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

//return forward
bool imgrgb::GroundCheck(bool &left, bool &right) {
	int ypos = floory * 512;
	int yposup = (floory - 20) * 512;
	left = groundmat.data[204 + ypos] && groundmat.data[102 + ypos] && groundmat.data[153 + yposup];
	right = groundmat.data[306 + ypos] && groundmat.data[408 + ypos] && groundmat.data[357 + yposup];
	return groundmat.data[256 + ypos] && groundmat.data[204 + ypos] && groundmat.data[306 + ypos];
}

void imgrgb::findballs(cv::Mat &hsvin, cv::Mat &circlemat) {
	circlemat.release();
	circlemat = cv::Mat::zeros(380,512,CV_8UC1);
	
	cv::Mat colorfilter;
	cv::inRange(hsvin, ballcolor-3, ballcolor+3,colorfilter);

	cv::Point Point1;
	Point1.x = 0;
	Point1.y = 0;
	cv::Point Point2;
	Point2.x = 512;
	Point2.y = 175;
	cv::rectangle(colorfilter,Point1,Point2, cv::Scalar(0,0,0),-1);

	//Clean it
	for(int i = 0; i < 5; i++)
		cv::morphologyEx(colorfilter, colorfilter, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2,2)));	

	std::vector<cv::KeyPoint> circlelist;
	balldet->detect(colorfilter,circlelist);

	//Searching area of each points
	std::vector<cv::Mat> croplist;
	for(int i = 0; i < circlelist.size(); i++) {

		if(!(circlelist[i].pt.x > 30 && circlelist[i].pt.x < 480 && circlelist[i].pt.y < 350 && circlelist[i].pt.y > 200))
			continue;

		std::vector<cv::Vec3f> circles;
		cv::Mat temp = hsvin(cv::Rect(circlelist[i].pt.x - 30, circlelist[i].pt.y - 30, 60, 60));
		cv::Mat croparea;
		cv::inRange(temp, ballcolor-10, ballcolor+10,temp);
		cv::Canny(temp, croparea, 5, 251,3 );
		if(cv::countNonZero(croparea) > 100)
			continue;
		cv::HoughCircles(croparea, circles, CV_HOUGH_GRADIENT, 1, 30, 1,15,0,300);

		//circle(circlemat, cv::Point(circlelist[i].pt.x,circlelist[i].pt.y), 10, cv::Scalar(255,255,255),CV_FILLED, 8,0);

		int ypos = (circlelist[i].pt.x + circlelist[i].pt.y*512);
		if(circles.size() != 0 && temp.data[ypos] != 0 && temp.data[ypos-3] != 0 && temp.data[ypos+3] != 0)
			circle(circlemat, cv::Point(circlelist[i].pt.x + circles[0][0] - 30, circlelist[i].pt.y + circles[0][1] - 30), circles[0][2], cv::Scalar(255,255,255),CV_FILLED, 8,0);

		croplist.push_back(croparea);
		croparea.copyTo(hsvin(cv::Rect(circlelist[i].pt.x - 30, circlelist[i].pt.y - 30, 60, 60)));
		temp.release();
		croparea.release();
	}

	colorfilter.release();
}

void imgrgb::ProcessImg(unsigned char *rgbbuff) {
	//Image needs to be scaled down
	//1920 -> 512, 1080 -> 381 with black pixels on the bottom, total 424
	//3.75x, 2.47x
	cv::Mat rgbin;
	cv::Mat HSVin;
	cv::Mat img(1080,1920, CV_8UC4, rgbbuff);
	//Resize and scale
	cv::resize(img,rgbin, cv::Size(650,381)); //512 * 1.27 x 424 * .9
	//cv::resize(img,rgbin, cv::Size(512,424)); //512 * 1.27 x 424 * .9

	//Empty image
	img = cv::Mat::zeros(424,512,CV_8UC4);
	//Crop the part of the image we need
	img = rgbin(cv::Rect(100, 0, 512, 381)); //position of square, size of square

	//cv::resize(img,rgbin, cv::Size(512,424));
	//img = rgbin;
	cv::flip(img, rgbin, 1);

	cv::cvtColor(rgbin,HSVin,CV_BGR2HSV);
	cv::Mat channels[3];
	cv::split(HSVin, channels);
	cv::Mat HVIMG;

	/*finding the ground*/
	HVIMG = channels[1]* .65 + channels[2]*.35;
	findground(HVIMG);
	cv::Mat circlesstuff;
	findballs(channels[0], circlesstuff);

	int resolution = krgb.width*381;
	for(int i = 0; i < resolution; i++) {
		/*
		krgb.data[i*4] = channels[0].data[i];
		krgb.data[i*4 + 1] = channels[0].data[i];
		krgb.data[i*4 + 2] = channels[0].data[i];
		*/
		krgb.data[i*4] = rgbin.data[i*4];
		krgb.data[i*4 + 1] = rgbin.data[i*4+1];
		krgb.data[i*4 + 2] = rgbin.data[i*4+2];


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
	HVIMG.release();
	//matcircle.release();

}

nimg *imgrgb::GetImg() {
	return &krgb;

}

