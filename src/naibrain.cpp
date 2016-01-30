#include "naibrain.h"

naibrain::naibrain() : pfind(&wmap) {
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

std::vector<unsigned char *> &naibrain::GetImages(unsigned int imgmask) {
	Images.clear();

	//Processing kinect
	if((imgmask & (KRGB | KDEP)) && kinect_manager != NULL) 
		if(kinect_manager->ProcessImages()) {
			if(imgmask & KDEP && kinect_manager->GetDepthImg())
				Images.push_back(kinect_manager->GetDepthImg());

			if(imgmask & KRGB && kinect_manager->GetRGBImg())
				Images.push_back(kinect_manager->GetRGBImg());
		}

	return Images;

}
