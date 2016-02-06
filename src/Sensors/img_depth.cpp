#include "Sensors/img_depth.h"
using namespace std;
bool kimgdep = true;
bool kimgedge = true;
bool kimgline = true;
bool kimgplane = true;


imgd::imgd() : 	iteration(3) {
	cv::SimpleBlobDetector::Params params;
	params.minDistBetweenBlobs = 0.0f;
	params.filterByInertia = false;
	params.filterByConvexity = false;
	params.filterByColor = false;
	params.filterByCircularity = false;
	params.filterByArea = true;
	params.filterByInertia = true;

	params.minArea = 1.0f;
	params.maxArea = 200.0f;
	params.minInertiaRatio = 0.3;
	params.maxInertiaRatio = 1.0;
	blob_detector = cv::SimpleBlobDetector::create(params);

	kdepth.width = 512;
	kdepth.height = 424;
	kdepth.depth = 3;
	kdepth.data = new unsigned char[kdepth.width * kdepth.height * kdepth.depth];

}

imgd::~imgd() {


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

	cv::Mat img(424, 512, CV_8UC1, stuff);
	cv::Mat outimg(424, 512, CV_8UC1);
	cv::Mat outimg2 = cv::Mat::zeros(424,512, CV_8UC1);

	try {
		cv::morphologyEx(img, outimg, cv::MORPH_OPEN, cv::Mat()); //Phasing out blobs
		cv::morphologyEx(outimg, img, cv::MORPH_CLOSE, cv::Mat()); //Closing gaps

		//blob_detector->detect(img, keypoints); //Detecting remainders
		cv::Canny(img, outimg, 30, 30, 3); //Detecting edges

		//for(int i = 0; i < keypoints.size(); i++)
		//	cv::circle(outimg, keypoints[i].pt, keypoints[i].size, cv::Scalar(0,0,0),-1); //Removing them
		cv::HoughLinesP(outimg, lines, 2, CV_PI/180, 30, 50, 10);
		//cv::dilate(outimg, outimg, cv::Mat(), cv::Point(0,0));

		for(int i = 0; i < lines.size(); i++ ) {
		    cv::Point pt1(lines[i][0],lines[i][1]), pt2(lines[i][2],lines[i][3]);
		    cv::line(outimg2, pt1, pt2, cv::Scalar(255,255,255), 3, 8);
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
	}

	outimg.release();
	outimg2.release();
	img.release();
}

nimg *imgd::GetImg() {
	return &kdepth;
}
