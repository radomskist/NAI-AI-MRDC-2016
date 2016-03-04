#include "Infoproc/states/argstate/scan_state.h"

scan_state::scan_state(const world_map *set_map, std::string set_args) : arg_state(set_map,set_args) {
	once = false;
	angle = 0;
	direction = true;
	turndon = false;

	std::vector<std::array<std::string,2>> tempargs = GetArgs();
	for(int i = 0; i < tempargs.size(); i++) {
		switch(tempargs[i][0][0]) {
			case 'd':
				if(tempargs[i][2][0] == 'l')
					direction = false;
				break;
			case 'a':
				angle = abs(std::stof(tempargs[i][2], std::string::size_type())); //always positive
				break;
			case 'o':
				if(tempargs[i][2][0] != '0')
					once = true;
				break;
			case 'f': 
				
				
				break;
		}
	}

	float setang = 0;
	//Turning left
	if(!direction) {
		scandir = std::string("RA+");
		if(wmap->GetRobot()->rot < angle || abs(wmap->GetRobot()->rot - angle) < .05)
			angle = wmap->GetRobot()->rot + 6.28 - angle;
	}
	else {
		if(wmap->GetRobot()->rot > angle || abs(wmap->GetRobot()->rot - angle) < .05)
			angle = wmap->GetRobot()->rot + 6.28 - angle;

		scandir = std::string("RA-");
	}


	if(angle >= 3.14) {
		scandir.append("1.57");
		angle -= 1.57;
	}
	else {
		angle = 0;
		scandir.append(std::to_string(angle));
	}

	//Telling the brain to rotate
	commlist = scandir;
	comred = true;
}

void scan_state::SetStat(std::string set) {
	if(set[0] != '0') {
		scandir = scandir.substr(0,2);

		if(angle == 0) {
			//TODO add scanning function if done and o
			sexit = 1;
			return;
		}
		else if(angle >= 3.14) {
			scandir.append("1.57");
			angle -= 1.57;
		}
		else {
			angle = 0;
			scandir.append(std::to_string(angle));
		}
	}
}

scan_state::~scan_state() {
	
	
}

int scan_state::Process() { //Process information
	//TODO scan direction
	return sexit;
}





