#include "Infoproc/states/read_qr.h"

read_qr::read_qr(const world_map* set_map, const kinectman* set_rgb) : base_state(set_map), kinectmanager(set_rgb) {

}

read_qr::~read_qr() {

}

int read_qr::Process() {

	//Request to scan image see how far off from the center we are
	//int distfc = 0; //distance from center
	//float dist;
	//std::string code; //qr code

	//dist = kinectmanager->qrscan(&distfc, &code);

	//Strafe to line up with image


}
