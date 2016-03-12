#include "Infoproc/states/open_door.h"


/***********************


ARDUINO TEAM IS DOING DOOR OPENING
FOR THIS YEAR


*************************/

open_door::open_door(const world_map* set_wmap, path_finding& set_pfind) : base_state(set_wmap), pfind(set_pfind) {
	//TODO check for doors in ajacent doors, and face it. if none exit with -1
}

open_door::~open_door() {


}

int open_door::Process() {
	//Raise arm

	//Go forward

	//Lower arm
	
	//Back up until either door is halfway or arm is seen

	//Turn 20 degrees to the left

	//Drive foward a little then go inside

}

void open_door::SetStat(std::string) {


}
