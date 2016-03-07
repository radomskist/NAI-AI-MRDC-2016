#include "naibrain.h"

naibrain::naibrain() : pfind(GetMap()), driveman(&pfind, wmap.GetRobot()), locsys(&driveman, wmap.GetRobot()) {
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
	states.push(new first_door(GetMap(), GetPfind()));
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
		int checkpoints[5] = {97336, 97436, 97536, 97636,97736}; //525 * 190 + 56*(i*100)
		for(int i = 0; i < 5; i++)
			checkpoints[i] = kinect_manager->GetDist(checkpoints[i]);
		wmap.checkplanes(checkpoints);

		bool left,right,front;
		//std::cout << "Path check" << std::endl;
		front = kinect_manager->PathCheck(left,right);
		//std::cout << "Drive manager check" << std::endl;
		locsys.SetComm(driveman.GetCurComm());
		locsys.SetChecks(front,left,right);
		//std::cout << "End" << std::endl;
	}
	bool stateend = false;
	/*Managing states*/
	if(states.size() != 0) {
		if(states.top()->IsExit()) {
			base_state *newstate = states.top()->endstate();
			delete states.top();
			states.pop();
			if(newstate != NULL) {
				states.push(newstate);
				}
			stateend = true;
		}

		states.top()->Process();
		pfind.checkpath();
		std::string command = states.top()->commands();
		/*Processing state commands*/
		if(command.size() > 2) {
			std::string subcheck = command.substr(0,2);

			if(subcheck == "RA" || subcheck == "MV")
				driveman.runcom(states.top()->commands());
			else if(subcheck == "SS") {
				base_state *newstate = new scan_state(GetMap(), command.substr(3,command.size()));
				states.push(newstate);
			}
			
		}

		int ticknum = driveman.tick();
		if(ticknum == 1)
			states.top()->SetStat(std::string("1"));
		else if(ticknum == 2) {
			base_state *newstate = new open_door(GetMap(), pfind);
			states.push(newstate);
		}
		else if(ticknum == 3) { /*in override mode*/
			std::cout << "Drive manager overwritten" << std::endl;
			if(stateend)
				driveman.SetOverride(false);
		}
	}

	/*approximation*/
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
