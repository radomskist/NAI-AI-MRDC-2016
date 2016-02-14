#include "Infoproc/driveman.h"

drive_man::drive_man() {
	delay = GetMilli();

	try {
		drivechip = naicom::createcomm("NaiDrive");
	}
	catch (nfail &e) {
		drivechip = NULL;
		std::cout << "Arduino initialization failed: " << e.what() << std::endl;
	}
}

bool drive_man::runcom(std::string &command) {

	if(command.empty() || drivechip == NULL || delay > GetMilli())
		return false;

	delay = GetMilli() + 200;

	if(movecheck(command))
		return drivechip->writecom(command);
	else
		std::cout << "Obstacle" << std::endl;
	return false;
}

void drive_man::SetChecks(bool sfront, bool sleft, bool sright) {
	front = sfront;
	left = sleft;
	right = sright;
}


bool drive_man::movecheck(std::string &command) const {
	//Checking if can move forward
	if(command[0] == 'm' && command[1] == 'v')
		return ((front && left) || (front && right));
	
	return true;
}

drive_man::~drive_man() {


}
