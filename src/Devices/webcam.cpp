#include "Devices/webcam.h"

/**************
FACTOR FUNCTION
**************/
nwebc *naiwebc::createwebcam(std::string ofname, int camtype) {
	
    int file;
    v4l2_capability cap;
	int iterator = 0;
	std::string directory = "/dev/video";

	while(true) {
		std::string cur_dir = directory + std::to_string(iterator);

		if((file = open(cur_dir.c_str(), O_RDONLY)) == -1){ 
			close(file);
			throw nfail("Camera not found.");
		    return NULL;
		}

	   if(ioctl(file, VIDIOC_QUERYCAP, &cap) == -1) {
			close(file);
			throw nfail("Could not access video capacities!");
		}
		std::string cardname(reinterpret_cast<char*>(&cap.card[0]));
		if(ofname == cardname) {
			close(file);

		if(camtype == 1)
			return new bcam(iterator);
		else
			return new nwebc(iterator);
		}
	iterator++;
	}
	return NULL;
}


/***************
WEB CAMERA
****************/
nwebc::nwebc(int index) {
	cap = new cv::VideoCapture(index);

    if(!cap->isOpened()) {
		throw nfail("Webcam index failed!");
		return;
	}
	camimg.flags = BCCAM;
	camimg.width = cap->get(CV_CAP_PROP_FRAME_WIDTH);
	camimg.height = cap->get(CV_CAP_PROP_FRAME_HEIGHT);
	camimg.depth = 3;
	camimg.data = new unsigned char[640*480*3];
		
}

nwebc::~nwebc() {
	cap->release();
	delete cap;
}

nimg *nwebc::GetImg() {
    cap->read(img);
	camimg.data = img.data;
	return &camimg;
}

/****************
BALL CAMERA
****************/
bcam::bcam(int index) : nwebc(index) {

}

bcam::~bcam() {
	
}

void bcam::SetColor(unsigned char set_colr, unsigned char set_sat) {
	color = set_colr;
	sat = set_sat;
}

nimg *bcam::GetImg() {
	img.release();
   	cap->read(img);
	cv::Mat HSVin[3];
	cv::split(img, HSVin);

	/*finding green ball*/
	cv::Mat colorfilter, colorfilter1;
	cv::inRange(HSVin[0], color - 30, color + 30,colorfilter);
	cv::inRange(HSVin[2], sat - 50, sat + 50,colorfilter1);
	//cv::inRange(HSVin[1], 0, 100,colorfilter);
	cv::bitwise_and(colorfilter,colorfilter1,colorfilter);

	/*finding post*/
	cv::inRange(HSVin[2], 0, 100,colorfilter1);
	int res = 640*480;
	for(int i = 0; i < res; i++) {
		camimg.data[i*3] = 0;
		camimg.data[i*3+1] = 0;
		camimg.data[i*3+2] = 0;

		if(colorfilter.data[i])
			camimg.data[i*3] = colorfilter.data[i];
		if(colorfilter1.data[i])
			camimg.data[i*3 + 1] = colorfilter1.data[i];

	}	

	HSVin[0].release();
	HSVin[1].release();
	HSVin[2].release();
	colorfilter.release();
	colorfilter1.release();

	return &camimg;
}

