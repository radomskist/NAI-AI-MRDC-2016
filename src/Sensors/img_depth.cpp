#include "Sensors/img_depth.h"
using namespace std;
bool kimgdep = true;
bool kimgedge = true;
bool kimgline = true;
bool kimgplane = true;


imgd::imgd() : kdepth(512,424,3) {
}

imgd::~imgd() {
}

std::vector<std::array<cv::Point,4>> imgd::CalculatePlanes(std::vector<cv::Vec4i> &lines, std::vector<std::array<cv::Point,2>> &returnlines) {

	std::vector<std::array<cv::Point,4>> returnvec;
	std::vector<std::array<cv::Point,2>> verticle;
	std::vector<float> angles;

	/*Translating points into lines*/
	for(int i = 0; i < lines.size(); i++) {
		std::array<cv::Point, 2> points;
		points[0] = cv::Point(lines[i][0],lines[i][1]);
		points[1] = cv::Point(lines[i][2],lines[i][3]);

		//Ignore noise on left edge
		if(points[0].x < 4)
			continue;

		/*seperate the lines based on angle*/
		float Angle = atan2(points[1].y - points[0].y,points[1].x - points[0].x);
		if(fabs(Angle) > 1.22f) { /*70 degrees*/

			if(points[1].y < points[0].y) { /*setting rightside up*/
				cv::Point temp = points[1];
				points[1] = points[0];
				points[0] = temp;
			}
			verticle.push_back(points);
		}
		else {
			if(points[1].x < points[0].x) { /*setting rightside up*/
				cv::Point temp = points[1];
				points[1] = points[0];
				points[0] = temp;
				}
			returnlines.push_back(points);
			angles.push_back(Angle);
		}
	}


	//Merging extra horizontal lines
	for(int i = 0; i < returnlines.size(); i++)
		for(int j = i; j < returnlines.size(); j++) {
			if(abs(angles[i] -angles[j]) > .5) //seeing if similar angle
				continue;

			if(abs(returnlines[i][0].y - returnlines[j][0].y) > 5) //Making sure lines are even on the same level
				continue;

			//if its within the mainline just remove
			if(returnlines[i][0].x < returnlines[j][0].x && returnlines[i][1].x > returnlines[j][1].x) {
				returnlines.erase(returnlines.begin() + j);
				angles.erase(angles.begin() + j);		
			}
			//Making sure they're close enough
			else if(fabs(abs(returnlines[j][0].x - returnlines[i][0].x)) < 20 || fabs(abs(returnlines[j][1].x - returnlines[i][1].x)) < 20 
			|| fabs(abs(returnlines[j][1].x - returnlines[i][0].x)) < 20 || fabs(abs(returnlines[j][1].x - returnlines[i][0].x)) < 20
			|| (returnlines[i][0].x < returnlines[j][1].x && returnlines[i][1].x > returnlines[j][0].x)) { //Checking if its within bounds

				if(returnlines[j][0].x < returnlines[i][0].x) 
					returnlines[i][0] = returnlines[j][0];

				if(returnlines[j][1].x > returnlines[i][1].x)
					returnlines[i][1] = returnlines[j][1];

			returnlines.erase(returnlines.begin() + j);
			angles.erase(angles.begin() + j);	
			}

		}

	//Merging extra verticle lines
	for(int i = 0; i < verticle.size(); i++)
		for(int j = i; j < verticle.size(); j++) {
			if(abs(verticle[i][0].x - verticle[j][0].x) > 5) //Making sure lines are even on the same level
				continue;

			//if its within the mainline just remove
			if(verticle[i][0].y < verticle[j][0].y && verticle[i][1].y > verticle[j][1].y) 
				verticle.erase(verticle.begin() + j);

			//Making sure they're close enough
			else if(fabs(abs(verticle[j][0].y - verticle[i][0].y)) < 20 || fabs(abs(verticle[j][1].y - verticle[i][1].y)) < 20 
			|| fabs(abs(verticle[j][1].y - verticle[i][0].y)) < 20 || fabs(abs(verticle[j][1].y - verticle[i][0].y)) < 20
			|| (verticle[i][0].y < verticle[j][1].y && verticle[i][1].y > verticle[j][0].y)) { //Checking if its within bounds

				if(verticle[j][0].y < verticle[i][0].y) 
					verticle[i][0] = verticle[j][0];

				if(verticle[j][1].x > verticle[i][1].x)
					verticle[i][1] = verticle[j][1];

			verticle.erase(verticle.begin() + j);
			}

		}

	/*going through verticle lines to see if any match*/
	for(int i = 0; i < verticle.size(); i++) {
		int halfsize = (verticle[i][0].y - verticle[i][1].y) * .4;
		int range = abs(verticle[i][1].x - verticle[i][0].x);

		for(int j = 0; j < verticle.size(); j++) {
			if(i == j)
				continue;

			if(abs(verticle[i][0].y - verticle[j][0].y) < halfsize) {
				//std::array<cv::Point,4> pushin;
				//pushin[0] = verticle[i][0];
				//pushin[1] = verticle[j][0];
				//pushin[2] = verticle[j][1];
				//pushin[3] = verticle[i][1];
				//returnvec.push_back(pushin);
			}
		}

		/*horizontal line testing*/	
		for(int j = 0; j < returnlines.size(); j++) {

			//If its closer to the bottom of the plane than top then throw it out
			int heighttest = abs(verticle[i][0].y - returnlines[j][0].y);
			int heighttest2 = abs(verticle[i][1].y - returnlines[j][0].y);			
			if(heighttest > heighttest2) //have to work upside down with images, less is higher
				continue;

			//Making sure the horizontal is not more than 75% the verticle lines height above it
			heighttest = verticle[i][1].y - returnlines[j][0].y;			
			heighttest2 = (verticle[i][1].y - verticle[i][0].y)*1.25;
			if(heighttest > heighttest2)
				continue;

			//make sure the verticle is not more than 75% the horizontals width away from it
			heighttest = abs(verticle[i][0].x - returnlines[j][1].x);
			heighttest2 = abs(verticle[i][0].x - returnlines[j][0].x);

			int heighttest3 = abs(returnlines[j][1].x - returnlines[j][0].x)*.75;

			if(heighttest > heighttest3 && heighttest2 > heighttest3)
				continue;

			cv::Point closest;
			if(heighttest > heighttest2) 
				closest = returnlines[j][0];
			else
				closest = returnlines[j][1];

			std::array<cv::Point,4> pushin;
			pushin[0] = verticle[i][0];
			pushin[1] = closest;
			pushin[2] = verticle[i][0];
			pushin[3] = closest;
			returnvec.push_back(pushin);
		}
	}

	//Combine the matrices for visuals
	returnlines.insert(returnlines.end(), verticle.begin(), verticle.end());
	return returnvec;
}

/*Two pixel slope check suggested by Joel*/
void imgd::ProcessImg(unsigned char *depthbuff) {

	//Casting data to a float, which is what it's suppose to be (instead of what it gives you for some reason)
	float *datahold = (float *)&depthbuff[4];

	unsigned char normalized;
	unsigned resolution = kdepth.width * kdepth.height;
	unsigned char *stuff = new unsigned char[resolution];

	/*flipping the image*/
	for(int j = 0; j < kdepth.height; j++) {
		int currentrow = j*kdepth.width;
		for(int i = 0; i < kdepth.width; i++) {

			////normalize kinect range to 255
			normalized = datahold[currentrow + (kdepth.width -1 - i)] * 0.06375f; //(4500.0f - 500.0f)/(255)
			stuff[currentrow + i] = normalized;
		}
	}

	vector<cv::KeyPoint> keypoints;
	vector<cv::Vec4i> lines;


	cv::Mat img(424, 512, CV_8UC1, stuff); //mat for depth image
	cv::Mat outimg(424, 512, CV_8UC1); //Mat for edges
	cv::Mat outimg2; //Mat for lines
	cv::Mat outimg3 = cv::Mat::zeros(424,512,CV_8UC1); //Mat for planes

	try {
		cv::morphologyEx(img, outimg, cv::MORPH_OPEN, cv::Mat()); //Phasing out blobs
		cv::morphologyEx(outimg, img, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(11, 11))); //Closing gaps

		cv::Canny(img, outimg2, 80, 80, 3, false); //Detecting edges
		cv::morphologyEx(outimg2, outimg, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(11, 11))); //Closing gaps 
		cv::dilate(outimg, outimg, cv::Mat(), cv::Point(0,0),1);

		outimg2.setTo(cv::Scalar(0,0,0));
		//for(int i = 0; i < keypoints.size(); i++)
		//	cv::circle(outimg, keypoints[i].pt, keypoints[i].size, cv::Scalar(0,0,0),-1); //Removing them
		cv::HoughLinesP(outimg, lines, 2, CV_PI/180, 30, 70, 10);

		std::vector<std::array<cv::Point,2>> pointset;
		std::vector<std::array<cv::Point,4>> planeset = CalculatePlanes(lines,pointset);

		for(int i = 0; i < pointset.size(); i++ ) {
		    cv::line(outimg2, pointset[i][0], pointset[i][1], cv::Scalar(255,255,255), 3, 8);
		}

		for(int i = 0; i < planeset.size(); i++ ) {
		    cv::line(outimg3, planeset[i][0], planeset[i][1], cv::Scalar(255,255,255), 3, 8);
		    cv::line(outimg3, planeset[i][1], planeset[i][2], cv::Scalar(255,255,255), 3, 8);
		    cv::line(outimg3, planeset[i][2], planeset[i][3], cv::Scalar(255,255,255), 3, 8);
		    cv::line(outimg3, planeset[i][3], planeset[i][0], cv::Scalar(255,255,255), 3, 8);
		}

	}
	catch(std::exception &e) {
		std::cout << e.what()  << std::endl;
	}

	//Converting the float into a proper RGB image (rather than 4 split up parts of one float)
	for(int i = 0; i < resolution; i++) {
		if(!kimgdep) {
			kdepth.data[i*3] = 0;
			kdepth.data[i*3 + 1] = 0;
			kdepth.data[i*3 + 2] = 0;
		}
		else {
			kdepth.data[i*3] = img.data[i];
			kdepth.data[i*3 + 1] = img.data[i];
			kdepth.data[i*3 + 2] = img.data[i];
		}

		if(kimgline && outimg2.data[i]) {
			kdepth.data[i*3] = 0;
			kdepth.data[i*3+1] = 255;
			kdepth.data[i*3+2] = 0;
		}
		else if(kimgedge && outimg.data[i]) {
			kdepth.data[i*3] = 255;
			kdepth.data[i*3+1] = 0;
			kdepth.data[i*3+2] = 0;
		}
		else if(kimgplane && outimg3.data[i]) {
			kdepth.data[i*3] = 0;
			kdepth.data[i*3+1] = 0;
			kdepth.data[i*3+2] = 255;
		}
	}

	img.release();
	outimg.release();
	outimg2.release();
	img.release();
	delete[] stuff;
}

nimg *imgd::GetImg() {
	return &kdepth;
}
