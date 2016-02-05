#include "Sensors/img_depth.h"
using namespace std;

imgd::imgd() : 	iteration(3) {
	/*Initializing image processing variables*/
	//TODO MAKE LOADABLE BY CONFIG FILE!
	/*Has to be multiple of 2 and not 3*/
}

imgd::~imgd() {


}


/*Two pixel slope check suggested by Joel*/
void imgd::ProcessImg(unsigned char *depthbuff) {

/*	OpenCv smoothing didn't work. Might try later*/
	cv::Mat img(424, 512, CV_8UC3, depthbuff);
	cv::Mat outimg;

	try {
		cv::Canny(img, outimg, 10, 10, 3);
		//cv::dilate(outimg, outimg, cv::Mat(), cv::Point(0,0));

	}
	catch(std::exception &e) {
		std::cout << e.what()  << std::endl;
	}

	int resolution = 512*424;
	for(int i = 0; i < resolution; i++) {
		if(outimg.data[i])
			depthbuff[i*3] = outimg.data[i];
			//depthbuff[i*3+1] = img.data[i*3+1];
			//depthbuff[i*3+2] = img.data[i*3+2];
	}
}

