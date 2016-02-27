#include "naibrain.h"

naibrain::naibrain() : pfind(GetMap()), driveman(&pfind, wmap.GetRobot()), locsys(driveman, wmap.GetRobot()) {
	kinect_manager = 0;
	//Initializing kinect manager
	//red = 175
	//blue = ~100
	//green = 50
	//painted green = 86
	ballcolor = 89;
	try {
		kinect_manager = new kinectman(ballcolor);
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
	states.push(new test_state(GetMap(), GetPfind()));
}

naibrain::~naibrain() {
	if(kinect_manager != NULL)
		delete kinect_manager;
}

bool naibrain::KStatus() {
	return kinect_manager;
}

const world_map *naibrain::GetMap() {
	return &wmap;
}

path_finding &naibrain::GetPfind(){
	return pfind;
}

void naibrain::gentest() {
	wmap.gentest();

}
void naibrain::tick() {
	if(kinect_manager != NULL) {
		//std::cout<< "Kinect" << std::endl;
		kinect_manager->ProcessImages();
		//std::cout << "Adding planes" << std::endl;

		wmap.AddPlanes(kinect_manager->GetPlanes());
		float checkpoints[5] = {56, 156, 256, 356,456};
		for(int i = 0; i < 5; i++)
			checkpoints[i] = kinect_manager->GetDist(checkpoints[i]);
		wmap.checkplanes(checkpoints);

		bool left,right,front;
		//std::cout << "Path check" << std::endl;
		front = kinect_manager->PathCheck(left,right);
		//std::cout << "Drive manager check" << std::endl;
		driveman.SetChecks(front,left,right);
		//std::cout << "End" << std::endl;
	}
	states.top()->Process();
	driveman.runcom(states.top()->commands());
	states.top()->SetStat(driveman.tick());

	obj_point dinc;
	float dang;
	if(locsys.approximate(dinc, dang)) 
		wmap.SetRobotAttr(dinc, dang);
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
