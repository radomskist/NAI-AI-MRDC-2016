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
		bcwebcam = NULL;
		std::cout << "Webcam initialization failed: " << e.what() << std::endl;
	}

	try {
		drivechip = naicom::createcomm("NaiDrive");
	}
	catch (nfail &e) {
		drivechip = NULL;
		std::cout << "Arduino initialization failed: " << e.what() << std::endl;
	}

	states.push(new test_state(GetMap()));
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

void naibrain::tick() {
	std::string currentcoms = states.top()->commands();

	if(kinect_manager != NULL)
		kinect_manager->ProcessImages();

	if(drivechip != NULL){
		std::string readit = drivechip->readall();

		if(!drivechip->writecom(currentcoms))
			std::cout << "Failed to write" << std::endl;
	}
}

std::vector<nimg*>  &naibrain::GetImages(unsigned int imgmask) {
	Images.clear();

	//Processing kinect
	if((imgmask & (KRGB | KDEP)) && kinect_manager != NULL) {
		if(imgmask & KDEP)
			Images.push_back(kinect_manager->GetDepthImg());

		if(imgmask & KRGB)
			Images.push_back(kinect_manager->GetRGBImg());
	}

	//Processing webcams
	if((imgmask & BCCAM) && bcwebcam != NULL)
		Images.push_back(bcwebcam->GetImg());

	return Images;

}
