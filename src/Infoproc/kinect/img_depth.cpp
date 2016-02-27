#include "Infoproc/kinect/img_depth.h"
using namespace std;

//TODO: clean up this disgusting lazyness
bool kimgdep = true;
bool kimgedge = true;
bool kimgline = true;
bool kimgplane = true;


imgd::imgd() : kdepth(512,424,3), filteredimg(424, 512, CV_8UC1) {

	//TODO make load from config
	horizontalchecky = 175; //How high should a line be to check if its the top of a wall
	floory = 300; 
	lineest = false; //Predict where lines might be?
	pixdist = 10; //Distance between pixels when testing flatness of plane
	slopeerrorrange = 2; //Range of error when seeing if plane is flat
	kdepth.flags = KDEP;
	failpercent = .1; //Percent of wall spots that can fail but still be accepted. To avoid noise problems

	kdepth.data = new unsigned char[kdepth.width * kdepth.height * kdepth.depth];
	freezetime = GetSec();
	floordist = 0;
}

imgd::~imgd() {

}

bool imgd::ScanGround(bool& left, bool& right) {
	unsigned int ydown = 512*floory;

	//Getting average distance of floor
	if(floordist == 0) {
		int avg = 0;
		for(int i = 0; i < 3; i++) 
			avg += averagepointsc(ydown + 200+(56*i));

		floordist = avg*.333f;

		if(floordist == 0)
			return false;
		else
			return true;
	}
	

	int count = 0;
	for(int i = 0; i < 2; i++) 
		count += (abs(averagepointsc(ydown + 102+56*i) - floordist) < 5);	//This might be dangerous

	left = (count == 2);
	count = 0;
	for(int i = 0; i < 2; i++) 
		count += (abs(averagepointsc(ydown + 354+56*i) - floordist) < 5);

	right = (count == 2);
	count = 0;
	for(int i = 0; i < 10; i++) 
		count += (abs(averagepointsc(ydown + 186+14*i) - floordist) < 4);

	return (count > 6);	
}

std::vector<std::array<cv::Point,4>> imgd::ProcessLines(std::vector<cv::Vec4i> &lines, std::vector<std::array<cv::Point,2>> &horizontal, std::vector<std::array<cv::Point,2>> &verticle) {

	std::vector<std::array<cv::Point,4>> returnvec;
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
			horizontal.push_back(points);
			angles.push_back(Angle);
		}
	}


	//Merging extra horizontal lines
	for(int i = 0; i < horizontal.size(); i++)
		for(int j = i; j < horizontal.size(); j++) {
			if(abs(angles[i] -angles[j]) > .35) //seeing if similar angle
				continue;

			if(abs(horizontal[i][0].y - horizontal[j][0].y) > 5) //Making sure lines are even on the same level
				continue;

			//if its within the mainline just remove
			if(horizontal[i][0].x < horizontal[j][0].x && horizontal[i][1].x > horizontal[j][1].x) {
				horizontal.erase(horizontal.begin() + j);
				angles.erase(angles.begin() + j);		
			}
			//Making sure they're close enough
			else if(abs(horizontal[j][0].x - horizontal[i][0].x) < 20 || abs(horizontal[j][1].x - horizontal[i][1].x) < 20 
			|| abs(horizontal[j][1].x - horizontal[i][0].x) < 20 || abs(horizontal[j][1].x - horizontal[i][0].x) < 20
			|| (horizontal[i][0].x < horizontal[j][1].x && horizontal[i][1].x > horizontal[j][0].x)) { //Checking if its within bounds

				if(horizontal[j][0].x < horizontal[i][0].x) 
					horizontal[i][0] = horizontal[j][0];

				if(horizontal[j][1].x > horizontal[i][1].x)
					horizontal[i][1] = horizontal[j][1];

			horizontal.erase(horizontal.begin() + j);
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
			else if(abs(verticle[j][0].y - verticle[i][0].y) < 20 || abs(verticle[j][1].y - verticle[i][1].y) < 20 
			|| abs(verticle[j][1].y - verticle[i][0].y) < 20 || abs(verticle[j][1].y - verticle[i][0].y) < 20
			|| (verticle[i][0].y < verticle[j][1].y && verticle[i][1].y > verticle[j][0].y)) { //Checking if its within bounds

				if(verticle[j][0].y < verticle[i][0].y) 
					verticle[i][0] = verticle[j][0];

				if(verticle[j][1].x > verticle[i][1].x)
					verticle[i][1] = verticle[j][1];

			verticle.erase(verticle.begin() + j);
			}

		}

	//Should we estimate where lines could be?
	if(!lineest)
		return returnvec;

	/*going through verticle lines to see if any match*/
	for(int i = 0; i < verticle.size(); i++) {
		int halfsize = (verticle[i][0].y - verticle[i][1].y) * .4;
		int range = abs(verticle[i][1].x - verticle[i][0].x);

		for(int j = i; j < verticle.size(); j++) {
			if(i == j)
				continue;

			if(abs(verticle[i][0].y - verticle[j][0].y) < halfsize) {
				std::array<cv::Point,4> pushin;
				pushin[0] = verticle[i][0];
				pushin[1] = verticle[j][0];
				pushin[2] = verticle[j][1];
				pushin[3] = verticle[i][1];
				returnvec.push_back(pushin);
			}
		}

		/*horizontal line testing*/	
		for(int j = i; j < horizontal.size(); j++) {

			//If its closer to the bottom of the plane than top then throw it out
			int heighttest = abs(verticle[i][0].y - horizontal[j][0].y);
			int heighttest2 = abs(verticle[i][1].y - horizontal[j][0].y);			
			if(heighttest > heighttest2) //have to work upside down with images, less is higher
				continue;

			//Making sure the horizontal is not more than 75% the verticle lines height above it
			heighttest = verticle[i][1].y - horizontal[j][0].y;			
			heighttest2 = (verticle[i][1].y - verticle[i][0].y)*1.25;
			if(heighttest > heighttest2)
				continue;

			//make sure the verticle is not more than 75% the horizontals width away from it
			heighttest = abs(verticle[i][0].x - horizontal[j][1].x);
			heighttest2 = abs(verticle[i][0].x - horizontal[j][0].x);

			int heighttest3 = abs(horizontal[j][1].x - horizontal[j][0].x)*.75;

			if(heighttest > heighttest3 && heighttest2 > heighttest3)
				continue;

			cv::Point closest;
			if(heighttest > heighttest2) 
				closest = horizontal[j][0];
			else
				closest = horizontal[j][1];

			std::array<cv::Point,4> pushin;
			pushin[0] = verticle[i][0];
			pushin[1] = closest;
			pushin[2] = verticle[i][0];
			pushin[3] = closest;
			returnvec.push_back(pushin);
		}
	}

	return returnvec;
}

std::vector<obj_plane> imgd::CalculatePlanes(std::vector<std::array<cv::Point,2>> &horizontal, std::vector<std::array<cv::Point,2>> &verticle, std::vector<std::array<cv::Point,4>> &returnvec) {
	std::vector<obj_plane> planelist;
	for(int i = 0; i < verticle.size(); i++) {
		for(int j = 0; j < horizontal.size(); j++) {

			if(abs(verticle[i][0].y - horizontal[j][0].y) > 20)//Checking if it's even on the same plane
				continue; 

			int px = abs(verticle[i][0].x - horizontal[j][0].x);
			int py = abs(verticle[i][0].x - horizontal[j][1].x);

			if(px > 20 && py > 20) 
				continue;

			cv::Point closest, furthest, corner;
			if(px > py) {
				closest = horizontal[j][1];
				furthest = horizontal[j][0];
			}
			else {
				closest = horizontal[j][0];
				furthest = horizontal[j][1];
			}				
		
			corner.x = furthest.x;
			int difference = ((abs(horizontal[j][0].y - horizontal[j][1].y))*.5);
			corner.y = verticle[i][1].y;
			closest.x = verticle[i][0].x;
			std::array<cv::Point,4> newpoint;

			/*organizing so it goes from left to right*/
			if(closest.x < furthest.x) {
				newpoint[0] = closest;
				newpoint[1] = verticle[i][1];
				newpoint[2] = corner;
				newpoint[3] = furthest;
			}
			else {
				newpoint[0] = furthest;
				newpoint[1] = corner;
				newpoint[2] = verticle[i][1];
				newpoint[3] = closest;
			}

			std::vector<std::array<cv::Point,4>> newarray;
			newarray.push_back(newpoint);	
			ConvertToObj(newarray, planelist);
			if(!newarray.empty())
				returnvec.insert(returnvec.end(), newarray.begin(), newarray.end());
		}
	}

	for(int i = 0; i < horizontal.size(); i++) {
		if(horizontal[i][0].y < horizontalchecky && horizontal[i][1].y < horizontalchecky && abs(horizontal[i][0].y - horizontal[i][1].y) < 75) {
			std::array<cv::Point,4> newarray;

			//Making sure is on the left
			if(horizontal[i][0].x < horizontal[i][1].x) {
				newarray[0] = horizontal[i][0];
				newarray[3] = horizontal[i][1];
			} else {
				newarray[0] = horizontal[i][1];
				newarray[3] = horizontal[i][0];
			}
			cv::Point addpoint = newarray[0];
			addpoint.y = addpoint.y + 80;
			newarray[1] = addpoint;

			addpoint = newarray[3];
			addpoint.y = addpoint.y + 80;
			newarray[2] = addpoint;

			returnvec.push_back(newarray);
		}
	}

	horizontal.insert(horizontal.end(),verticle.begin(),verticle.end());
	return planelist;
}

void imgd::ConvertToObj(std::vector<std::array<cv::Point,4>> &processplane, std::vector<obj_plane>& returnplane) {

	int checkdist = abs(processplane[0][3].x - processplane[0][0].x - 10);
	int checkdistv = abs(processplane[0][0].y - processplane[0][1].y - 10); //Verticle check
	
	if(pixdist > checkdist || pixdist > checkdistv) {
		processplane.erase(processplane.begin());
		return;
	}

	/*horizontal*/
	int checktot = checkdist / pixdist;

	/*verticle*/
	int checktottwo = checkdistv / pixdist;

	cv::Point checkpoint;
	int midy = processplane[0][0].y + (processplane[0][1].y - processplane[0][0].y) * 0.5;
	int midx = processplane[0][0].x + (processplane[0][3].x - processplane[0][0].x) * 0.5;
	checkpoint.y = midy;

	//Boundries pixels should be between
	//Subtracting 5 pixels to make sure it's not on the edge which would be 0.

	/*Horizontal check*/
	cv::Point getextreme;
	getextreme.x = processplane[0][3].x - pixdist;
	getextreme.y = midy;
	std::array<cv::Point, 4> newpoint;
	newpoint[0] = getextreme;
	int extremetwo = averagepoints(getextreme);

	getextreme.x = processplane[0][0].x + pixdist;
	newpoint[1] = getextreme;
	int extremeone = averagepoints(getextreme);


	/*Verticle check*/
	getextreme.x = midx;
	getextreme.y = processplane[0][0].y + pixdist;
	newpoint[2] = getextreme;
	int extremethree = averagepoints(getextreme);

	getextreme.y = processplane[0][1].y - pixdist;
	newpoint[3] = getextreme;
	int extremefour = averagepoints(getextreme);

	/*
	THIS DRAWS THE MIDLINE FOR DEBUGGING*/	
	processplane.push_back(newpoint);


	if(extremeone < 5 || extremetwo < 5 || extremethree < 5 || extremefour < 5) {
		processplane.erase(processplane.begin());
		return;
	}

	//extreme one should be the smaller range
	if(extremetwo < extremeone) {
		int temp = extremetwo;
		extremetwo = extremeone;
		extremeone = temp;
	}
	//Verticle fix
	if(extremefour < extremethree) {
		int temp = extremefour;
		extremefour = extremethree;
		extremethree = temp;
	}

	//Adding range of error
	extremeone -= slopeerrorrange;
	extremetwo += slopeerrorrange;
	extremethree -= slopeerrorrange;
	extremefour += slopeerrorrange;

	//TODO check if multiple walls where in one plane?

	//Checking if wall is just a gap (horizontal)
	int failcount = 0;
	int failcap = checktot * failpercent;
	for(int i = 1; i < checktot; i++) {
		checkpoint.x = processplane[0][0].x + i * pixdist;
		int checkvalue = averagepoints(checkpoint);
		if(checkvalue < extremeone || checkvalue > extremetwo)
			failcount++;
	}

	if(failcount > failcap){
		processplane.erase(processplane.begin());
		return;
	}

	//Checking for gaps (verticle)
	failcount = 0;
	failcap = checktottwo * failpercent;
	checkpoint.x = midx;

	for(int i = 1; i < checktottwo; i++) {
		checkpoint.y = processplane[0][0].y + i * pixdist;
		int checkvalue = averagepoints(checkpoint);
		if(checkvalue < extremethree || checkvalue > extremefour) 
			failcount++;
	}

	if(failcount > failcap){
		processplane.erase(processplane.begin());
		return;
	}

	/*Doing freezeframe*/
	if(processplane.size() > 1 && freezetime < GetSec()) {
		kdepth.flags = KDEP | KFREEZE;
		freezetime = GetSec() + 1;
	}

	//half xkinect FOV = 35.3
	/***********************
	CONVERTING TO OBJ_PLANE
	***********************/
	// 0.616101226 = 35.3 (half of the kinects FOV) converted to radians
	// the position of the point on the screen frm the center of the verticle axis
	// .003906 = 1/256, which is half of the width (512)
	obj_point pointhold;
	obj_plane newplane(1,1);
	//Only check top 2 points
	for(int i = 0; i < 2; i++) {
		int depthspot;

		//TODO: bottom points to see if too distorted to be a wall?
		if(i == 0)
			depthspot = processplane[0][0].x + 5 + ((processplane[0][0].y + 5)* kdepth.width);
		else
			depthspot = processplane[0][3].x - 5 + ((processplane[0][3].y + 5)* kdepth.width);

		pointhold.y = ((256.0f - processplane[0][i*3].x)/256.0f) * (averagepoints(depthspot) * sin(0.616101226)); //Distance from center

		float pointval = averagepoints(depthspot);
		pointhold.x = sqrt(abs(pointval*pointval - (processplane[0][i*3].x * processplane[0][i*3].x))); //forward

		if(i == 0) {
			pointhold.z = 300;
			newplane.p[0] = pointhold;
			pointhold.z = 0;
			newplane.p[1] = pointhold;
		}
		else {
			pointhold.z = 300;
			newplane.p[3] = pointhold;
			pointhold.z = 0;
			newplane.p[2] = pointhold;
		}

	}

	returnplane.push_back(newplane);
}

float imgd::GetDist(unsigned int atpos) {
	return averagepoints(atpos);
}

inline unsigned char imgd::averagepointsc(unsigned int point) {
	//Checking if in range
	if(kdepth.width - (point / kdepth.width) < 3 || (point / kdepth.width) < 3)
		return 0;

	//Reverse calculating filter because noise is too bad on kinect
	int total = filteredimg.data[point];
	total += filteredimg.data[point + 1];
	total += filteredimg.data[point - 1];
	total *= 0.33f;

	return total;
}


inline float imgd::averagepoints(unsigned int point) {
	//Checking if in range
	if(kdepth.width - (point / kdepth.width) < 3 || (point / kdepth.width) < 3)
		return 0;

	//Reverse calculating filter because noise is too bad on kinect
	float total = filteredimg.data[point];
	total += filteredimg.data[point + 1];
	total += filteredimg.data[point - 1];
	total *= 5.228757647  ; //(4500.0f - 500.0f)/(255) * 1/3

	return total;
}


inline int imgd::averagepoints(cv::Point avg) {
	//Checking if in range
	if(kdepth.width - avg.x < 3 || avg.x < 3)
		return 0;

	int yspot = avg.y*kdepth.width;
	int total = filteredimg.data[avg.x + yspot];

	total += filteredimg.data[avg.x + 1 + yspot];
	total += filteredimg.data[avg.x - 1 + yspot];
	total *= 0.33f;

	return total;
}

void imgd::ProcessImg(unsigned char *depthbuff,std::vector<obj_plane> &planeset) {
	//Casting data to a float, which is what it's suppose to be (instead of what it gives you for some reason)
	datahold = (float *)&depthbuff[4];

	kdepth.flags = KDEP;
	unsigned char normalized;
	unsigned resolution = kdepth.width * kdepth.height;
	
	/*flipping the image*/
	for(int j = 0; j < kdepth.height; j++) {
		int currentrow = j*kdepth.width;
		for(int i = 0; i < kdepth.width; i++) {
			////normalize kinect range to 255
			normalized = datahold[currentrow + (kdepth.width -1 - i)] * 0.06375f; //(255)/(4500.0f - 500.0f)
			filteredimg.data[currentrow + i] = normalized;
		}
	}
	vector<cv::KeyPoint> keypoints;
	vector<cv::Vec4i> lines;

	cv::Mat outimg(424, 512, CV_8UC1); //Mat for edges
	cv::Mat outimg2; //Mat for lines
	cv::Mat outimg3 = cv::Mat::zeros(424,512,CV_8UC1); //Mat for planes

	try {
		//Doing some serious noise cleaning
		cv::morphologyEx(filteredimg, outimg, cv::MORPH_OPEN, cv::Mat()); //Phasing out blobs
		cv::morphologyEx(outimg, filteredimg, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(11, 11))); //Closing gaps

		cv::Canny(filteredimg, outimg2, 80, 80, 3, false); //Detecting edges
		cv::morphologyEx(outimg2, outimg, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(11, 11))); //Closing gaps 
		cv::dilate(outimg, outimg, cv::Mat(), cv::Point(0,0),1);
		outimg2.setTo(cv::Scalar(0,0,0));

		//for(int i = 0; i < keypoints.size(); i++)
		//	cv::circle(outimg, keypoints[i].pt, keypoints[i].size, cv::Scalar(0,0,0),-1); //Removing them
		cv::HoughLinesP(outimg, lines, 2, 0.017, 30, 70, 10);
		std::vector<std::array<cv::Point,2>> pointset; //horizontal points
		std::vector<std::array<cv::Point,2>> vertset; //verticle points

		//Image to plane processing is done in these two functions
		std::vector<std::array<cv::Point,4>> planepoints = ProcessLines(lines,pointset,vertset); //turning lines into planes
		planeset = CalculatePlanes(pointset,vertset,planepoints); //converting image planes into obj planes (also check if valid)

		//This is all for image drawing
		for(int i = 0; i < pointset.size(); i++ ) {
		    cv::line(outimg2, pointset[i][0], pointset[i][1], cv::Scalar(255,255,255), 3, 8);
		}

		for(int i = 0; i < planepoints.size(); i++ ) {
		    cv::line(outimg3, planepoints[i][0], planepoints[i][1], cv::Scalar(255,255,255), 3, 8);
		    cv::line(outimg3, planepoints[i][1], planepoints[i][2], cv::Scalar(255,255,255), 3, 8);
		    cv::line(outimg3, planepoints[i][2], planepoints[i][3], cv::Scalar(255,255,255), 3, 8);
		    cv::line(outimg3, planepoints[i][3], planepoints[i][0], cv::Scalar(255,255,255), 3, 8);
		}

	}
	catch(std::exception &e) {
		std::cout << e.what()  << std::endl;
	}

	for(int i = 0; i < resolution; i++) {
		if(!kimgdep) {
			kdepth.data[i*3] = 0;
			kdepth.data[i*3 + 1] = 0;
			kdepth.data[i*3 + 2] = 0;
		}
		else {
			kdepth.data[i*3] = filteredimg.data[i];
			kdepth.data[i*3 + 1] = filteredimg.data[i];
			kdepth.data[i*3 + 2] = filteredimg.data[i];
		}

		if(kimgplane && outimg3.data[i]) {
			kdepth.data[i*3] = 0;
			kdepth.data[i*3+1] = 0;
			kdepth.data[i*3+2] = 255;
		}
		else if(kimgline && outimg2.data[i]) {
			kdepth.data[i*3] = 0;
			kdepth.data[i*3+1] = 255;
			kdepth.data[i*3+2] = 0;
		}
		else if(kimgedge && outimg.data[i]) {
			kdepth.data[i*3] = 255;
			kdepth.data[i*3+1] = 0;
			kdepth.data[i*3+2] = 0;
		}

	}
	outimg.release();
	outimg2.release();
	outimg3.release();
}

nimg *imgd::GetImg() {
	return &kdepth;
}
