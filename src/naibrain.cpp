#include "naibrain.h"

naibrain::naibrain() : pfind(GetMap()) {
	kinect_manager = 0;
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
		bcwebcam = naiwebc::createwebcam("USB Camera (046d:08ad)");
	}
	catch (nfail &e) {
		std::cout << "Webcam initialization failed: " << e.what() << std::endl;
	}

	try {
		drivechip = naicom::createcomm("NaiDrive");
	}
	catch (nfail &e) {
		std::cout << "Arduino initialization failed: " << e.what() << std::endl;
	}
}

naibrain::~naibrain() {
	if(kinect_manager != NULL)
		delete kinect_manager;
}

bool naibrain::KStatus() {
	return kinect_manager;
}

const world_map &naibrain::GetMap() {
	return wmap;
}

const path_finding &naibrain::GetPfind(){
	return pfind;
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
	if((imgmask & BCCAM) && bcwebcam != NULL)
		Images.push_back(bcwebcam->GetImg());

	return Images;

}
