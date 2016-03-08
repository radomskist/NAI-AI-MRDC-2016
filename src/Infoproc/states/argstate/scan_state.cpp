#include "Infoproc/states/argstate/scan_state.h"

scan_state::scan_state(const world_map *set_map, std::string set_args) : arg_state(set_map,set_args) {
	once = false;
	angle = 0;
	direction = true;
	init = false;
	mode = 0;

	std::vector<std::array<std::string,2>> tempargs = GetArgs();
	for(int i = 0; i < tempargs.size(); i++) {
		switch(tempargs[i][0][0]) {
			case 'd':
				if(tempargs[i][1][0] == 'l')
					direction = false;
				break;
			case 'a':
				angle = abs(std::stof(tempargs[i][1], std::string::size_type())); //always positive
				break;
			case 'o':
				if(tempargs[i][1][0] != '0')
					once = true;
				break;
			case 'f': 
				//searching for door
				if(tempargs[i][1][0] == 'd')
					scantarg = "F S 90";
				
				break;
		}
	}

	//Turning left
	if(!direction) {
		scandir = std::string("Rr+");
		if(wmap->GetRobot()->rot < angle || abs(wmap->GetRobot()->rot - angle) < .05)
			angle = wmap->GetRobot()->rot + 6.28 - angle;
	}
	else {
		if(wmap->GetRobot()->rot > angle || abs(wmap->GetRobot()->rot - angle) < .05)
			angle = wmap->GetRobot()->rot + 6.28 - angle;

		scandir = std::string("Rr-");
	}

	if(angle >= 3.14) {
		scandir.append("157!");
		angle -= 157;
	}
	else 
		scandir.append(std::to_string(100*angle).substr(0,3) + "!");

	//Telling the brain to rotate
	//commlist = scandir;
	commlist = scantarg;
	comred = true;
}

void scan_state::SetStat(std::string set) {

	if(set[0] != '0') {
		scandir = scandir.substr(0,2);

		mode++;
		if(angle < 5) {
			//Doing our scan once (TODO TEST)
			if(once && mode == 1) {
				comred = true;
				return;
			}
			sexit = 1;
			return;
		}
		else if(angle >= 314) {
			scandir.append("157!");
			angle -= 157;
		}
		else {
			angle = 0;
			scandir.append(std::to_string(angle) + "!");
		}
	}
}

scan_state::~scan_state() {
	
	
}

int scan_state::Process() { //Process information
	if(!init && comred == false) {
		init = true;
		commlist = scantarg;
	}

	//TODO process 45 degree angles
	if(!once)
		comred = true;

	return sexit;
}





