#include "Infoproc/states/find_door.h"

first_door::first_door(const world_map* set_map, path_finding &set_pfind) : base_state(set_map), pfind(set_pfind) {
	readytoopen = false;
	qrread = false;
	scanattempt = false;
	scanning = false;
	readingqr = false;
	
}
first_door::~first_door() {



}

void first_door::intialize() {
	const std::vector<obj_cube>& entities = wmap->GetEnts();

	//TODO Do a scan of the area
	if(!scanattempt){
		commlist = "SS a:0;f:d;"
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
}

int first_door::Process() {
	if(!readytoopen && !scanning) {
		intialize();
		return;
	}

	if(!qrread) {
		commlist = "RQ;"
		comred = true;
		return;
	}
	//Do go through door state

	//Pop
}

void first_door::SetStat(std::string set_state) {
	/*are we checking if a scan attempt is done?*/
	if(set_state[0] != '0') {
		if(scanning) {
			scanning = false;
			scanattempt = true;
		}
		else if(readingqr){
			readingqr = false;
			qrread = true;
		}
	}
}




