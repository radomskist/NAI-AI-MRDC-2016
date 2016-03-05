#include "Infoproc/driveman.h"

drive_man::drive_man(const path_finding * set_pfind, const obj_cube *set_rob) :  pfind(set_pfind), robot(set_rob) {
	delaytime = GetMilli();
	dir = 0;
	currentnode = 0;
	estiangle = 0;
	estimv.x = 0;
	estimv.y = 0;
	cpathid = 0;
	est = false;
	overridemode = false;

	/*Callibrations*/
	delay = 150; // in milliseconds
	drivespeed = 183*.001*delay; //speed per seconds converted for delay
	turnspeed = .56*.001*delay;
	turntol = .09; //Turning tolerance before considered to be straight

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

	return false;
}

void drive_man::override(bool set) {
	overridemode = set;
}

const std::string drive_man::GetCurComm() {
	return currentpath;
}

bool drive_man::tick() {
	//TODO readd override for when the path is completed
	if(pfind->GetPath().size() == 0)
		return false;

	//Is it a new path or is path finished?
	if(pfind->GetPathID() != cpathid) { //If new path
		currentnode = pfind->GetPath().size();
		cpathid = pfind->GetPathID();
	}

	if(currentnode == 0) 
		return true;
	

	const std::vector<obj_point> &curpath = pfind->GetPath();
	/***Checking if path is valid***/
	if(curpath.size() < 2)
		return false;

	difference = 0;
	unsigned int milli = GetMilli();
	if(delaytime < milli) {
		delaytime = milli + delay;
	}
	else
		return false;

	/*if we're rotating see if we're on track or not*/
	if((currentpath[0] == 'R')) 
		if(fabs(dir - robot->rot) < turntol) {
			commandhist.push_back(std::string("MV!"));
			currentpath = "MV 90!";
		}

	/*Are we in new node?*/
	if(((dir < .09  || abs(dir - 3.14) < .09)&& (abs(curpath[currentnode-1].x - robot->pos.x) < 20))
	 || ((abs(dir - 1.57) < .09 || abs(dir - 4.71) < .09 ) && (abs(curpath[currentnode-1].y - robot->pos.y) < 20))) {

		currentnode--;
		if(currentnode == 0)
			return true;

		if(abs(curpath[currentnode].x - curpath[currentnode-1].x) != 0)
			curpath[currentnode].x < curpath[currentnode-1].x ? dir = 0 : dir = 3.14;
		else
			curpath[currentnode].y < curpath[currentnode-1].y ? dir = 1.57 : dir = 4.71;


		//Getting direction
		float dircompare = abs(dir - robot->rot);
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
			if(robot->rot > 3.14) {
				anchor = robot->rot - 3.14;
				if(robot->rot > dir && dir > anchor) {
					currentpath = "RR 20!";
					ss << "RR " << robot->rot - dir;
				}
				else if (robot->rot < dir){
					currentpath = "RL 20!";
					ss << "RL " << dir - robot->rot;
				}
				else {
					currentpath = "RL 20!";
					ss << "RR " << ((3.14+robot->rot) - (3.14+dir));
				}
			}
			/*Less than 180*/
			else {
				anchor = robot->rot + 3.14;
				if(robot->rot < dir && dir < anchor) {
					currentpath = "RL 20!";
					ss << "RL " << dir - robot->rot;
				}
				else if (robot->rot > dir){
					currentpath = "RR 20!";
					ss << "RR " << robot->rot - dir;
				}
				else {
					currentpath = "RR 20!";
					ss << "RR " << ((3.14+dir) - (3.14+robot->rot));
				}
			}
			commandhist.push_back(ss.str());
		}
		/******************************************/
	/*logging where we're going*/
	//commandhist
	}

	if(!overridemode)
		execcom();
	//else
	//	execcom(overridecom);

	return false;
}

void drive_man::execcom() {
	if(drivechip != NULL)	 //Checking here so we can do simulations
		drivechip->writecom(currentpath);

	if(currentpath[0] == 'M' && currentpath[1] == 'V') {
		if(currentpath[3] == '9') {
			int estimove = drivespeed;
			if(abs(dir - 4.71) < .05 || abs(dir - 1.57) < .05)
				(abs(dir - 1.57) < .05) ? estimv.y += estimove : estimv.y -= estimove;
			else
				(dir < .05) ? estimv.x += estimove : estimv.x -= estimove;
		}
		else if(currentpath[3] == '1' && currentpath[4] == '8') {
			int estimove = drivespeed;
			if(abs(dir - 4.71) < .05 || abs(dir - 1.57) < .05)
				(abs(dir - 1.57) < .05) ? estimv.y += estimove : estimv.y -= estimove;
			else
				(dir < .05) ? estimv.x += estimove : estimv.x -= estimove;
		}
	}
	else if(currentpath[0] == 'R' && currentpath[1] == 'R') {
		float estang = turnspeed;
		estiangle -= estang;
	}
	else if(currentpath[0] == 'R' && currentpath[1] == 'L'){
		float estang = turnspeed;
		estiangle += estang;
	}
	est = true;
}

void drive_man::execcom(std::string &setstring) {
	if(setstring.size() != 0)
		return;

	if(drivechip != NULL)	 //Checking here so we can do simulations
		drivechip->writecom(setstring);

	if(setstring[0] == 'M' && setstring[1] == 'V') {
		if(setstring[3] == '9') {
			int estimove = drivespeed;
			if(abs(dir - 4.71) < .05 || abs(dir - 1.57) < .05)
				(abs(dir - 1.57) < .05) ? estimv.y += estimove : estimv.y -= estimove;
			else
				(dir < .05) ? estimv.x += estimove : estimv.x -= estimove;
		}
		else if(setstring[3] == '1' && setstring[4] == '8') {
			int estimove = drivespeed;
			if(abs(dir - 4.71) < .05 || abs(dir - 1.57) < .05)
				(abs(dir - 1.57) < .05) ? estimv.y += estimove : estimv.y -= estimove;
			else
				(dir < .05) ? estimv.x += estimove : estimv.x -= estimove;
		}
	}
	else if(setstring[0] == 'R' && setstring[1] == 'R') {
		float estang = turnspeed;
		estiangle -= estang;
	}
	else if(setstring[0] == 'R' && setstring[1] == 'L'){
		float estang = turnspeed;
		estiangle += estang;
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

drive_man::~drive_man() {


}
