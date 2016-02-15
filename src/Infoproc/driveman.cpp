#include "Infoproc/driveman.h"

drive_man::drive_man(const path_finding * set_pfind) :  pfind(set_pfind) {
	delay = GetMilli();
	dir = 0;
	try {
		drivechip = naicom::createcomm("NaiDrive");
	}
	catch (nfail &e) {
		drivechip = NULL;
		std::cout << "Arduino initialization failed: " << e.what() << std::endl;
	}
	currentnode = 0;
}

bool drive_man::runcom(std::string &command) {
	if(command.empty() || drivechip == NULL)
		return false;

	override = command;
	return false;
}

void drive_man::SetChecks(bool sfront, bool sleft, bool sright) {
	front = sfront;
	left = sleft;
	right = sright;
}

void drive_man::tick() {
	if(drivechip == NULL)
		return;

	//TODO readd override for when the path is completed
	//also need checks for path being completed
	checkpath();

	if(delay > GetMilli())
		delay = GetMilli() + 200;

	if(movecheck(currentpath))
		drivechip->writecom(currentpath);
	else
		std::cout << "Obstacle" << std::endl;
}

bool drive_man::checkpath() {
	//Return whether or not there is a path
	//TODO generate tag to know if path is updated or not
	//if(currentnode
	
	
}

bool drive_man::movecheck(std::string &command) {
	//Checking if can move forward
	if(dir == 90)
		return ((front && left) || (front && right));
	
	return true;
}

drive_man::~drive_man() {


}
