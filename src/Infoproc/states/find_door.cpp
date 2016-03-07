#include "Infoproc/states/find_door.h"

first_door::first_door(const world_map* set_map, path_finding &set_pfind) : base_state(set_map), pfind(set_pfind) {
	readytoopen = false;
	qrread = false;
	scanattempt = false;
	scanning = false;
	readingqr = false;
	turning = false;
	turndone = false;
	infrontdoor = false;
	startingpos = false;
}
first_door::~first_door() {



}

void first_door::intialize() {
	const std::vector<obj_cube>& entities = wmap->GetEnts();

	//TODO test
	if(!startingpos) {
		startingpos = true;
		pfind.gotopoint(obj_point(200,2200,50));
		return;
		}

	else if(!scanattempt){
		commlist = "SS a:0;f:d;";
		comred = true;
		return;
	}

	//Seeing if scan picked up door
	for(int i = 0; i < entities.size(); i++) {
		std::cout << entities[i].GetName() << std::endl;
		if(entities[i].GetName() != "door")
			continue;
		//Door found;

		float rotation = entities[i].rot;
		int gx = entities[i].pos.x;
		int gy = entities[i].pos.y;

		//seeing which way door is facing
		if(rotation < .05)
			gx += 400;
		else if(abs(rotation - 1.54) < .05)
			gx += 400;
		else if(abs(rotation - 3.14) < .05) 
			gx -= 400;
		else if(abs(rotation - 4.71) < .05)
			gx -= 400;
		else
			continue;

		if(pfind.gotopoint(obj_point(gx,gy,50)))
			readytoopen = true;
		else
			//failed do scan
			std::cout << "TODO: SCAN WHEN FAILED" << std::endl;

		return;
	}
	//Door not found
	pfind.gotopoint(obj_point(600,2600,50));
}

int first_door::Process() {
	if(!readytoopen && !scanning) {
		intialize();
		return 0;
	}

	//TODO makesure facing door
	if(!turndone) {
		float turnangle = 6.24 - wmap->GetRobot()->rot;
		commlist = "RA+";
		commlist.append(std::to_string(turnangle));
		comred = true;
		turning = true;
		return 0;
	}

	if(!qrread) {
		commlist = "RQ;";
		comred = true;
		return 0;
	}
	sexit = 1;
}

base_state *first_door::endstate() {
	return new go_inside(wmap,pfind);
}

void first_door::SetStat(std::string set_state) {
	/*are we checking if a scan attempt is done?*/
	if(set_state[0] != '0') {
		if(!infrontdoor) 
			infrontdoor = true;
		else if(scanning) {
			scanning = false;
			scanattempt = true;
		}
		else if(turning) {
			turning = false;
			turndone = true;
		}
		else if(readingqr){
			readingqr = false;
			qrread = true;
		}
	}
}




