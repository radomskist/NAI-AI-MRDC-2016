#include "naibrain.h"

naibrain::naibrain() : pfind(&wmap) {
	kinect_manager = 0;

	std::cout << camcount() << std::endl;
	//Initializing kinect manager
	try {
		kinect_manager = new kinectman();
	}
	catch (nfail &e) {
		std::cout << "Kinect initialization failed: " << e.what() << std::endl;
		delete kinect_manager;
		kinect_manager = NULL; //just incase
	}

	//Initializing webcam
	try {
		bcwebcam = new nwebc(0);
	}
	catch (nfail &e) {
		std::cout << "Webcam initialization failed: " << e.what() << std::endl;
		delete bcwebcam;
		bcwebcam = NULL; //just incase
	}
}

/*this hack is literally the only way to get the camera count*/
int naibrain::camcount() {
	//VideoCapture capture( CV_CAP_DSHOW );
	//capture.VI.listDevices();
}

naibrain::~naibrain() {
	delete kinect_manager;
}

bool naibrain::KStatus() {
	return kinect_manager;
}

world_map &naibrain::GetMap() {
	return wmap;
}

std::vector<obj_point> naibrain::GetPath(obj_point point) {
	return pfind.gotopoint(point);
}

std::vector<nimg*>  &naibrain::GetImages(unsigned int imgmask) {
	Images.clear();

	//Processing kinect
	if((imgmask & (KRGB | KDEP)) && kinect_manager != NULL) 
		if(kinect_manager->ProcessImages()) {
			if(imgmask & KDEP && kinect_manager->GetDepthImg())
				Images.push_back(kinect_manager->GetDepthImg());

			if(imgmask & KRGB && kinect_manager->GetRGBImg())
				Images.push_back(kinect_manager->GetRGBImg());
		}

	//Processing webcams
	if((imgmask & BCCAM) && bcwebcam != NULL) {
		Images.push_back(bcwebcam->GetImg());
	}

	return Images;

}
