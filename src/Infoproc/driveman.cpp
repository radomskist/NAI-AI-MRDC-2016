#include "Infoproc/driveman.h"

drive_man::drive_man(const path_finding * set_pfind, const obj_cube *set_rob) :  pfind(set_pfind), robot(set_rob) {
	delay = GetMilli();
	dir = 0;
	currentnode = -1;
	estiangle = 0;
	estimv.x = 0;
	estimv.y = 0;
	try {
		drivechip = naicom::createcomm("NaiDrive");
	}
	catch (nfail &e) {
		drivechip = NULL;
		std::cout << "Arduino initialization failed: " << e.what() << std::endl;
	}
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
	//TODO readd override for when the path is completed
	//also need checks for path being completed
	if(pfind->GetPath().size() == 0 || currentnode == 0)
		return;

	//TODO compensate simulation for delay due to lag
	if(delay < GetMilli())
		delay = GetMilli() + 250;
	else
		return;

	checkpath();

	if(movecheck(currentpath)) {
		if(drivechip != NULL)	 //Checking here so we can do simulations
			drivechip->writecom(currentpath);

		if(currentpath[0] == 'M' && currentpath[1] == 'V')
			(dir == 90 || dir == 270) ? estimv.x += 50 : estimv.y += 50;
		else if(currentpath[0] == 'R' && currentpath[1] == 'R')
			estiangle += 10;
		else if(currentpath[0] == 'R' && currentpath[1] == 'L')
			estiangle -= 10;
	}
	else
		std::cout << "Obstacle" << std::endl;
}

//TODO remove error-proneness (which came first)
void drive_man::GetEst(obj_point &GetMv, float &GetAng) {
	GetMv = estimv;
	GetAng = estiangle;
	estiangle = 0;
	estimv.x = 0;
	estimv.y = 0;
}

bool drive_man::checkpath() {
	const std::vector<obj_point> &curpath = pfind->GetPath();
	if(curpath.size() < 2)
		return false;

	//Return whether or not there is a path
	//TODO generate tag to know if path is updated or not
	if(currentnode == -1)
		currentnode = curpath.size() - 1;

	//TODO only do this for when on new node
	if((curpath[currentnode].x - curpath[currentnode-1].x) != 0)
		curpath[currentnode].x < curpath[currentnode-1].x ? dir = 90 : dir = 270;
	else
		curpath[currentnode].y < curpath[currentnode-1].y ? dir = 0 : dir = 180;

	if(((dir == 90 || dir == 270 ) && (abs(curpath[currentnode-1].x - robot->pos.x) < 50))
	 || ((dir == 0 || dir == 180) && (abs(curpath[currentnode-1].y - robot->pos.y) < 50)))
		currentnode--;

	//TODO add left for between 270 and 0
	if(dir - robot->rot < 2)
		currentpath = "MV 90!";
	else {
		if(dir > robot->rot)
			currentpath = "RR 20!";
		else
			currentpath = "RL 20!";

	}

}

bool drive_man::movecheck(std::string &command) {
	//Checking if can move forward
	if(dir == 90)
		return ((front && left) || (front && right));
	
	return true;
}

drive_man::~drive_man() {


}
