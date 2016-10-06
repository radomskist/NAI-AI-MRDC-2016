/*****************************************************************
	* Author: Steven Radomski radomskist@yahoo.com
	*
	* Copyright (C) 2017 Steven Radomski
	* 
	* This file was part of the 2016 NIU robotics AI robot project
	* 
	* This code cannot be copied or used without the permission of
	* the author
	*
	*
*****************************************************************/
#include "Infoproc/kinect/img_rgb.h"

imgrgb::imgrgb(unsigned char set_color) {
	floory = 323; //TODO make it load from config file

	ballcolor = set_color;
	krgb.flags = KRGB;
	krgb.width = 512;
	krgb.height = 424;
	krgb.depth = 4;
	krgb.data = new unsigned char[krgb.width*krgb.height*krgb.depth];
	groundmat = cv::Mat::zeros(380,512,CV_8UC1);
	objloc = cv::Mat::zeros(380,512,CV_8UC1);
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

	cv::SimpleBlobDetector::Params squareparam;
	squareparam.minDistBetweenBlobs = 30.0f;
	squareparam.filterByInertia = true;
	squareparam.filterByConvexity = false;
	squareparam.filterByColor = true;
	squareparam.filterByCircularity = false;
	squareparam.filterByArea = true;
	squareparam.minCircularity = .65;
	squareparam.maxCircularity = .85;
	squareparam.minArea = 5.0f;
	squareparam.maxArea = 600.0f;
	squareparam.blobColor = 255;
	squareparam.minInertiaRatio = 0.95f;
	squareparam.maxInertiaRatio =  1.0f;
	squareparam.minConvexity = .95;
	squareparam.maxConvexity = 1.0; 

	squaredet = cv::SimpleBlobDetector::create(squareparam);

	zbarscan.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);

}

imgrgb::~imgrgb() {

}

void imgrgb::qrscan(int *offset, std::string *code_text) {

    zbar::Image zimage(512, 424, "Y800",  (char*)hsvchan[0].data, 512 * 424);

    // scan the image for barcodes
    int n = zbarscan.scan(zimage);
	std::cout << n << std::endl;
    // extract results
    for(zbar::Image::SymbolIterator i = zimage.symbol_begin(); i != zimage.symbol_end(); ++i) {
        // do something useful with results
        std::cout << "decoded " << i->get_type_name()
             << " symbol \"" << i->get_data() << '"' << std::endl;
    }

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

cv::Point2f imgrgb::FindObjColor(unsigned char objhue,unsigned char objsat) {
	cv::Mat colorfilter;
	cv::Mat colorfilter1;
	objloc.release();
	objloc = cv::Mat::zeros(380,512,CV_8UC1);

	//filtering out all the colors
	cv::inRange(hsvchan[0], objhue - 20, objhue + 20,colorfilter);
	cv::inRange(hsvchan[1], objsat - 50, objsat + 30,colorfilter1);
	cv::bitwise_and(colorfilter,colorfilter1,colorfilter);
	//cv::morphologyEx(colorfilter, colorfilter, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4,4)));
	//cv::morphologyEx(colorfilter, colorfilter, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7,7)));
	//cv::morphologyEx(colorfilter, colorfilter, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4,4)));
	//outlining whats left
	hsvchan[0] = colorfilter.clone();
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(colorfilter,contours,hierarchy,cv::CHAIN_APPROX_NONE,cv::RETR_LIST);
	
	int maxsize = -1;
	int ipos = -1;

	//Returning largest square shaped block (due to small noise)
	for(int i = 0; i < contours.size(); i++) {
		int csize = cv::contourArea(contours[i]);

		//making sure ration of bounding rectangle is square-like
		cv::Rect boundrect = boundingRect(contours[i]);
		float bratio = boundrect.width/(float)boundrect.height;

		if(bratio < .9 || bratio > 1.1)
			continue;

		//Seeing how filled the box is and how large it is by scaling size by fullness
		csize *= (1 - (csize/(boundrect.width *  boundrect.height))*.9);

		if(maxsize < csize) {
			maxsize = csize;
			ipos = i;
			}
	}

	if(ipos == -1) {
		colorfilter.release();
		colorfilter1.release();
		return cv::Point2f(-1,-1);
	}

	cv::Moments conmoments = cv::moments(contours[ipos]);
	cv::Point2f centerofsquare((conmoments.m10/conmoments.m00), (conmoments.m01/conmoments.m00));
	circle(objloc, centerofsquare ,20, cv::Scalar(255,255,255),-1,8,0);


	colorfilter.release();
	colorfilter1.release();
	return centerofsquare;
}

//return forward
bool imgrgb::GroundCheck(bool &left, bool &right) {
	int ypos = floory * 512;
	int yposup = (floory - 46) * 512;
	left = (groundmat.data[50 + yposup] + groundmat.data[100 + yposup] + groundmat.data[150 + yposup]) > 510;
	right = (groundmat.data[462 + yposup] + groundmat.data[412 + yposup] + groundmat.data[362 + yposup]) > 510;

	int returnfront = 0;
	for(int i = 0; i < 8; i++) {
		if(groundmat.data[32 + ypos + (56*i)])
			returnfront++;
		if(groundmat.data[150 + yposup + (15*i)])
			returnfront++;
	}
	return (returnfront > 15);
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
	hsvchan[0].release();
	hsvchan[1].release();
	hsvchan[2].release();
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
	cv::split(HSVin, hsvchan);
	cv::Mat HVIMG;

	
	/*finding the ground*/
	HVIMG = hsvchan[1]* .65 + hsvchan[2]*.35;
	findground(HVIMG);
	//FindObjColor(65,100); TESTING QR FINDER
	//cv::Mat circlesstuff;
	//findballs(hsvchan[0], circlesstuff);

	int resolution = krgb.width*381;
	for(int i = 0; i < resolution; i++) {
		/*
		krgb.data[i*4] = hsvchan[0].data[i];
		krgb.data[i*4 + 1] = hsvchan[0].data[i];
		krgb.data[i*4 + 2] = hsvchan[0].data[i];
		*/

		krgb.data[i*4] = rgbin.data[i*4];
		krgb.data[i*4 + 1] = rgbin.data[i*4+1];
		krgb.data[i*4 + 2] = rgbin.data[i*4+2];


		//if(cannystuff.data[i])
		//	krgb.data[i*4 + 2] = cannystuff.data[i];	

		//krgb.data[i*4 + 3] = 0;
		if(objloc.data[i])
			krgb.data[i*4 + 1] = objloc.data[i];
		/*if(circlesstuff.data[i]) {
			krgb.data[i*4] = 0;
			krgb.data[i*4 + 1] = circlesstuff.data[i];
			krgb.data[i*4 + 2] = 0;
		}
		else if(groundmat.data[i]) {
			krgb.data[i*4] = groundmat.data[i];
			krgb.data[i*4 + 1] = 0;
			krgb.data[i*4 + 2] = 0;
		}*/
	}
	//cannystuff.release();
	img.release();
	rgbin.release();
	HVIMG.release();
	//matcircle.release();

}

nimg *imgrgb::GetImg() {
	return &krgb;

}

