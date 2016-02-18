#include "Infoproc/driveman.h"

drive_man::drive_man(const path_finding * set_pfind, const obj_cube *set_rob) :  pfind(set_pfind), robot(set_rob) {
	delaytime = GetMilli();
	dir = 0;
	currentnode = 0;
	estiangle = 0;
	estimv.x = 0;
	estimv.y = 0;
	cpathid = 0;
	front = true;
	left = true;
	right = true;
	est = false;

	/*Callibrations*/
	delay = 150; // in milliseconds
	drivespeed = 183*.001*delay; //speed per seconds converted for delay
	turnspeed = 32*.001*delay;
	turntol = 5; //Turning tolerance before considered to be straight

	try {
		drivechip = naicom::createcomm("NaiDrive");
	}
	catch (nfail &e) {
		drivechip = NULL;
		std::cout << "Arduino initialization failed: " << e.what() << std::endl;
	}
}


//Overriding commands
bool drive_man::runcom(std::string &command) {
	if(command.empty() || drivechip == NULL)
		return false;

	override = command;
	return false;
}

//Can we go left, front, or right?
void drive_man::SetChecks(bool sfront, bool sleft, bool sright) {
	front = sfront;
	left = sleft;
	right = sright;
}

bool drive_man::tick() {
	//TODO readd override for when the path is completed

	if(pfind->GetPath().size() == 0)
		return false;

	//Is it a new path or is path finished?
	if(currentnode == 0) {
		if(pfind->GetPathID() != cpathid) { //If new path
			currentnode = pfind->GetPath().size();
			cpathid = pfind->GetPathID();
		}
		else 
			return true;
	}

	const std::vector<obj_point> &curpath = pfind->GetPath();
	/***Checking if path is valid***/
	if(curpath.size() < 2)
		return false;

	difference = 0;
	unsigned int milli = GetMilli();
	if(delaytime < milli) {
		difference = (milli/delaytime); //divided by 250
		delaytime = milli + delay;
	}
	else
		return false;

	/*if we're rotating see if we're on track or not*/
	if((currentpath[0] == 'R'))  {
		if(abs(dir - robot->rot) < turntol) {
			commandhist.push_back(std::string("MV!"));
			currentpath = "MV 90!";
		}
	}

	/*Are we in new node?*/
	if(((dir == 90 || dir == 270 ) && (abs(curpath[currentnode-1].x - robot->pos.x) < 50))
	 || ((dir == 0 || dir == 180) && (abs(curpath[currentnode-1].y - robot->pos.y) < 50))) {
		currentnode--;
		if(currentnode == 0)
			return true;

		if((curpath[currentnode].x - curpath[currentnode-1].x) != 0)
			curpath[currentnode].x < curpath[currentnode-1].x ? dir = 90 : dir = 270;
		else
			curpath[currentnode].y < curpath[currentnode-1].y ? dir = 0 : dir = 180;

		//Getting direction
		int dircompare = dir - robot->rot;
		/*driving foward if within tolerance*/
		if(abs(dircompare) < turntol) {
			commandhist.push_back(std::string("MV!"));
			currentpath = "MV 90!";
		}

		/*****************************************
		figuring out which way to rotate otherwise*/
		else {
			int moveto;
			int anchor = 0; //halfway across circle
			std::stringstream ss;

			/*Greater than 180*/
			if(robot->rot > 180) {
				anchor = robot->rot - 180;
				if(robot->rot > dir && dir > anchor) {
					currentpath = "RL 20!";
					ss << "RL " << robot->rot - dir;
				}
				else if (robot->rot < dir){
					currentpath = "RR 20!";
					ss << "RR " << dir - robot->rot;
				}
				else {
					currentpath = "RR 20!";
					ss << "RR " << ((180+robot->rot) - (180+dir));
				}
			}
			/*Less than 180*/
			else {
				anchor = robot->rot + 180;
				if(robot->rot < dir && dir < anchor) {
					currentpath = "RR 20!";
					ss << "RR " << dir - robot->rot;
				}
				else if (robot->rot > dir){
					currentpath = "RL 20!";
					ss << "RL " << robot->rot - dir;
				}
				else {
					currentpath = "RL 20!";
					ss << "RL " << ((180+dir) - (180+robot->rot));
				}
			}
			commandhist.push_back(ss.str());
		}
		/******************************************/
	/*logging where we're going*/
	//commandhist
	}

	if(obstvoid())
		execcom();

	return false;
}

void drive_man::execcom() {
	if(drivechip != NULL)	 //Checking here so we can do simulations
		drivechip->writecom(currentpath);

	if(currentpath[0] == 'M' && currentpath[1] == 'V') {
		int estimove = difference * drivespeed;
		if(dir == 270 || dir == 90)
			(dir == 90) ? estimv.x += estimove : estimv.x -= estimove;
		else
			(dir == 0) ? estimv.y += estimove : estimv.y -= estimove;
	}
	else if(currentpath[0] == 'R' && currentpath[1] == 'R') {
		float estang = turnspeed*difference;
		estiangle += estang;
	}
	else if(currentpath[0] == 'R' && currentpath[1] == 'L'){
		float estang = turnspeed*difference;
		estiangle -= estang;
	}
	est = true;
}

//TODO remove error-proneness (which came first)
bool drive_man::GetEst(obj_point &GetMv, float &GetAng) {
	if(!est) 
		return false;

	GetMv = estimv;
	GetAng = estiangle;
	estiangle = 0;
	estimv.x = 0;
	estimv.y = 0;
	est = false;
	return true;
}

//obsticalavoid
//TODO Makesure doens't go off map
//TODO make sure not in state mode
bool drive_man::obstvoid() {
	//Checking if can move forward
	if(currentpath[0] == 'M' && currentpath[1] == 'V') {

		if (!((front && left) || (front && right))) {
			return false;
		}
		//else if(!right)
		//	currentpath = "RL 20!";
		//else
		//	currentpath = "RR 20!";
	}
	return true;

}




drive_man::~drive_man() {


}
