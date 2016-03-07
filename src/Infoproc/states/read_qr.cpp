#include "Infoproc/states/read_qr.h"

read_qr::read_qr(const world_map* set_map, kinectman& set_rgb) : base_state(set_map), kinectmanager(set_rgb) {

}

read_qr::~read_qr() {

}

int read_qr::Process() {

	//Request to scan image see how far off from the center we are
	float distz = 0; //dist from bot
	int distfc = 0; //distance from center
	std::string code; //qr code

	distz = kinectmanager.qrscan(&distfc, &code);

	if(abs(distfc) > 40) {
		//Strafe to line up with image

	}

}
