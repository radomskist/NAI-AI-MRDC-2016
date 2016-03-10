#include "Infoproc/states/read_qr.h"

read_qr::read_qr(const world_map* set_map, kinectman& set_kinect) : base_state(set_map), kinect_manager(set_kinect) {

}

read_qr::~read_qr() {

}

int read_qr::Process() {

	std::cout << "QR SCNANIN" << std::endl;
	//Request to scan image see how far off from the center we are
	float distz = 0; //dist from bot
	int distfc = 0; //distance from center
	std::string code; //qr code

	distz = kinect_manager.qrscan(&distfc, &code);

	if(abs(distfc) > 40) {
		//Strafe to line up with image
	}

}
