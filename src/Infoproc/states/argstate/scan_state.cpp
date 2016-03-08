#include "Infoproc/states/argstate/scan_state.h"

scan_state::scan_state(const world_map *set_map, std::string set_args, kinectman& set_kinect) : arg_state(set_map,set_args), kinect_manager(set_kinect) {
	once = false;
	angle = 0;
	direction = true;
	init = false;
	liningup = false;
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
				if(tempargs[i][1][0] == 'd'){
					shue = 65;
					ssat = 75;
				}
				
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
	commlist = scandir;
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
		if(liningup)
			liningup = false;
	}
}

scan_state::~scan_state() {
	
	
}

void scan_state::processscan() {
	float dist = kinect_manager.findobj(&offset,shue,ssat);
	std::cout << dist << " away and " << offset << "missaligned." << std::endl;

	int strafeamount;
	std::string strafedir;
	strafeamount = (offset * 0.00390625) + (dist * 0.113777778); //1/256,percentage of offset, 512/4500 cam width / depth distance

	if(offset > 0)
		strafedir = " 0 ";
	else
		strafedir = " 314 ";

	/*if(strafeamount > 20) {
		commlist = "MV";
		commlist.append(strafedir);
		commlist.append(std::to_string(strafeamount));
		commlist.append("!");
		liningup = true;
		comred = true;
	}
	else*/ if (dist > 700) {
		commlist = "MV";
		commlist.append(" 157 100!");
		liningup = true;
		comred = true;
	}
}

int scan_state::Process() { //Process information
	if(!init && comred == false) {
		init = true;
		commlist = "";
	}

	//TODO process 45 degree angles
	if(!once && !liningup) 
		processscan();

	return sexit;
}





