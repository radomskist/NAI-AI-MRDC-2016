#include "Infoproc/states/argstate/scan_state.h"

scan_state::scan_state(const world_map *set_map, std::string set_args, kinectman& set_kinect) : arg_state(set_map,set_args), kinect_manager(set_kinect) {
	once = false;
	angle = 0;
	direction = true;
	init = false;
	liningup = false;
	close = false;
	mode = 0;
	failcount = 0;
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
					ssat = 100;
					endcom = "QR";
				}
				
				break;
		}
	}

	//Turning left
	if(!once) {
		if(direction) {
			scandir = std::string("Rr+");
			if(wmap->GetRobot()->rot < angle || abs(wmap->GetRobot()->rot - angle) < .05)
				angle = wmap->GetRobot()->rot + 6.28 - angle;
		}
		else {
			if(wmap->GetRobot()->rot > angle || abs(wmap->GetRobot()->rot - angle) < .05)
				angle = wmap->GetRobot()->rot + 6.28 - angle;

			scandir = std::string("Rr-");
		}
	}
	else {
		if(direction)
			scandir = std::string("Rr+");
		else
			scandir = std::string("Rr-");
	}

	if(angle == 0) {
		commlist = "";
		mode == 1;
		return;
	}
	else if(!once && angle >= 314) {
		scandir.append("157!");
		angle -= 157;
	}
	else {
		scandir.append(std::to_string(angle).substr(0,3) + "!");
		angle = 0;
	}
	//Telling the brain to rotate
	commlist = scandir;
	comred = true;
}

void scan_state::SetStat(std::string set) {

	if(set[0] != '0') {
		if(mode == 0 && angle > 5) {
			scandir = scandir.substr(0,2);

			if(angle >= 314) {
				scandir.append("157!");
				angle -= 157;
			}
			else {
				scandir.append(std::to_string(angle) + "!");
				angle = 0;
			}
			commlist = scandir;
		}
		else if(mode == 0 && angle < 5)
			mode++;
		else if(mode == 1) {
			if(liningup)
				liningup = false;
		}
		else if(mode == 2) 
			mode++;
		else if(mode == 3) {
			sexit = 1;
			return;
		}
	}
}

scan_state::~scan_state() {
	
	
}

void scan_state::processscan() {
	float dist = kinect_manager.findobj(&offset,shue,ssat);

	if(dist == -1) {
		std::cout << "Obj scan failed" << std::endl;
		return;
	}

	int strafeamount;
	std::string strafedir;
	float modifier = .5;

	if(close)
		modifier = .15;

	//Checking if failed twice due to random values
	if(dist < 300)
		failcount++;
	else
		failcount = 0;

	if(failcount > 2)
		close = true;

	strafeamount = abs(offset*modifier); //1/256,percentage of offset, 512/4500 cam width / depth distance

	if(offset < 0)
		strafedir = " 314 ";
	else
		strafedir = " 0 ";

	if((close && strafeamount > 4) || (strafeamount > 37)) {
		commlist = "MV";
		commlist.append(strafedir);
		commlist.append(std::to_string(strafeamount));
		commlist.append("!");
		liningup = true;
		comred = true;
	}
	else if(kinect_manager.straighten()) {
		commlist = "ST";
		liningup = true;
		comred = true;
	}
	else if (dist > 200 || (!close && dist == 0)) {
		commlist = "MV";
		commlist.append(" 157 50!");
		liningup = true;
		comred = true;
	}
	else if(close) {
		std::cout << "exit mode" << std::endl;
		if(endcom.size() != 0) {
			commlist = endcom;
			comred = true;
			mode++;
		}
		else
			mode += 2;
	}
}

int scan_state::Process() { //Process information
	//TODO process 45 degree angles
	if(!liningup && mode == 1) 
		processscan();

	return sexit;
}

