#include "Infoproc/driveman.h"

drive_man::drive_man(const path_finding * set_pfind, const obj_cube *set_rob) :  pfind(set_pfind), robot(set_rob) {
	delaytime = GetMilli();
	dir = 0;
	turnamount = 0;
	currentnode = 0;
	estiangle = 0;
	estimv.x = 0;
	estimv.y = 0;
	cpathid = 0;
	est = false;
	obstical = false;
	isturning = false;
	overridemode = 0;
	continuestring = "g"; // string to keep going

	/*Callibrations*/
	delay = 150; // in milliseconds
	//drivespeed = 183*.001*delay; //speed per seconds converted for delay
	//turnspeed = .56*.001*delay;
	drivespeed = 170*.004*delay; //speed per seconds converted for delay
	turnspeed = .56*.004*delay;
	turntol = .09; //Turning tolerance before considered to be straight

	try {
		drivechip = naicom::createcomm("NaiDrive");
	}
	catch (nfail &e) {
		drivechip = NULL;
		std::cout << "Arduino initialization failed: " << e.what() << std::endl;
	}
}


void drive_man::SetObst(bool set_obst) {
	obstical = set_obst;

}


std::string drive_man::ArdState() {
	std::string ardstuff;

	if(drivechip != NULL)	 //Checking here so we can do simulations
		ardstuff = drivechip->readall();

	if(ardstuff.size() <= 0)
		return ardstuff;

	if(ardstuff[0] == 'q' && (overridemode == 2 || overridemode == 6)) 
		overridecom = "";

	return ardstuff;
}

//Overriding commands
bool drive_man::runcom(std::string &rcommand) {

	if((rcommand.size() < 2 || drivechip == NULL) && !overridemode)
		return false;

	if(rcommand[0] == 'R' && rcommand[1] == 'r') {
		std::cout << "Override 2 command: " << rcommand << std::endl;
		overridecom = rcommand;
		drivechip->writecom(overridecom);
		overridemode = 2; //wait till command is over
		return true;
	}
	else if(rcommand[0] == 'M' && rcommand[1] == 'V') {
		int divider;
		for(int i = 3; i < rcommand.size(); i++)
			if(rcommand[i] == ' ') {
				divider = i;
				break;
			}

		overridecom = rcommand.substr(0,divider) + "!";
		movedist = std::stoi(rcommand.substr(divider,rcommand.size()));
		std::cout << "Override drive command: " << currentpath << " distance:" << movedist  << std::endl;
		execcom(currentpath);
		overridemode = 4; //wait till command is over
		return true;
	}
	else if(rcommand[0] == 'S' && rcommand[1] == 'T') {
		std::cout << "Straightening" << std::endl;
		overridemode = 8;
		return true;
	}
	return false;
}


const std::string drive_man::GetCurComm() {
	return currentpath;
}

int drive_man::tick() {
	switch (overridemode) {
			ArdState();
		case 0 :
			break;

		case 2 : {
			if(drivechip == NULL || overridecom.size() == 0) { /*Drivechip == NULL For simulations*/
				est = true;
				estiangle = dir - robot->rot;
				overridemode = 0;

				if(!isturning) 
					return 5;

				isturning = false;
				/*check for door*/
				overridemode = 7;
			}
			break;
		}

		case 4 : {
			unsigned int milli = GetMilli();
			if(delaytime < milli) {
				delaytime = milli + delay;
			}
			else
				return 3;

			if(overridecom[0] == 'M' && overridecom[1] == 'V') {
				int drivedir = std::stoi(overridecom.substr(3,7));
				drivedir += ((robot->rot - 1.57)* 100);

				if(abs(drivedir - 471) < 5 || abs(drivedir - 157) < 5)
					 (abs(drivedir - 157) < 5) ? estimv.y += drivespeed : estimv.y -= drivespeed;
				else if (abs(drivedir) < 5 || abs(drivedir - 314) < 5)
					abs(drivedir < 5) ? estimv.x += drivespeed : estimv.x -= drivespeed;
			}

			if(movedist <= 0){
				overridecom = "";
				overridemode = 0;
				return 1;
			}
			else {
				drivechip->writecom(overridecom);
				movedist -= drivespeed;
			}
			return 3;
			break;
		}

		case 6 : {
			if(overridecom.size() == 0) { /*Drivechip == NULL For simulations*/
				est = true;
				overridemode = 0;
				if(abs(dir - 471) < 5 || abs(dir - 157) < 5)
					 (abs(dir - 157) < 5) ? estimv.y += 200 : estimv.y -= 200;
				else if (abs(dir) < 5 || abs(dir - 314) < 5)
					abs(dir < 5) ? estimv.x += 200 : estimv.x -= 200;
			}
			else if(drivechip != NULL)
				drivechip->writecom(continuestring);

			return 5;
			break;
		}

		case 7 : {
			int find;
			if (!(find = pfind->doorcheck())) {
				overridemode = 0;
				return 5;
			}

			overridemode = 6;
			if(drivechip == NULL) {
				if(find == 2)
					std::cout << "Open door." << std::endl;
				else
					std::cout << "Ram door." << std::endl;
				return 6;
			}
			else {
				//is it facing us?
				if(find == 1) {
					std::string opensesame("OS!");
					drivechip->writecom(opensesame);
				}
				//else ram
				else {
					std::string ramcommand("MV 157 400");
					runcom(ramcommand);
					return 4;
				}
			}
		}
		case 8: {
			if(abs(turnamount) < .5) {
				overridemode = 0;
				return 1;
			}

			unsigned int milli2 = GetMilli();
			if(delaytime < milli2)
				delaytime = milli2 + delay;

			turnamount -= turnspeed;
			std::string newstr;
			if(turnamount < 0)
				newstr = "RL 20";
			else
				newstr = "RR 20";

			if(drivechip != NULL) 
				drivechip->writecom(newstr);
			else {
				std::cout << "Straighening" << std::endl;
				overridemode = 0;
			}
			
			break;
		}

		default: 
			//ERROR!
			overridecom = "";
			overridemode = 0;
			std::cout << "==============================" << std::endl;
			std::cout << "ERROR: Incorrect override mode" << std::endl;
			return 3;
			break;
	}

	if(obstical)
		return 2;
	//TODO readd override for when the path is completed
	if(pfind->GetPath().size() == 0)
		return 0;

	//Is it a new path or is path finished?
	if(pfind->GetPathID() != cpathid) { //If new path
		currentnode = pfind->GetPath().size();
		cpathid = pfind->GetPathID();
	}

	if(currentnode == 0) 
		return 1;
	

	const std::vector<obj_point> &curpath = pfind->GetPath();
	/***Checking if path is valid***/
	if(curpath.size() < 2)
		return 0;

	difference = 0;
	unsigned int milli = GetMilli();
	if(delaytime < milli) {
		delaytime = milli + delay;
	}
	else
		return 0;

	/*if we're rotating see if we're on track or not
	if((currentpath[0] == 'R')) 
			commandhist.push_back(std::string("MV!"));
			currentpath = "MV 157!";
		}*/

	/*Are we in new node?*/
	if(((dir < .09  || abs(dir - 3.14) < .09)&& (abs(curpath[currentnode-1].x - robot->pos.x) < 50))
	 || ((abs(dir - 1.57) < .09 || abs(dir - 4.71) < .09 ) && (abs(curpath[currentnode-1].y - robot->pos.y) < 50))) {

		currentnode--;
		if(currentnode == 0)
			return 1;

		if(abs(curpath[currentnode].x - curpath[currentnode-1].x) != 0)
			curpath[currentnode].x < curpath[currentnode-1].x ? dir = 0 : dir = 3.14;
		else
			curpath[currentnode].y < curpath[currentnode-1].y ? dir = 1.57 : dir = 4.71;

		//Getting direction
		float dircompare = abs(dir - robot->rot);
		/*driving foward if within tolerance*/
		if(abs(dircompare) < turntol) {
			commandhist.push_back(std::string("MV!"));
			currentpath = "MV 157!";
		}

		/*****************************************
		figuring out which way to rotate otherwise*/
		else {
			int moveto;
			int anchor = 0; //halfway across circle
			std::stringstream ss;
			ss << "Rr " << (robot->rot - dir) << "!";
			overridemode = 2;
			isturning = true;
			commandhist.push_back(ss.str());
		}
		/******************************************/
		/*logging where we're going*/
		//commandhist

		/*Is the a door coming up?*/
		int find;
		if (currentnode >  1 && (find = pfind->doorcheck())) {
			overridemode = 6;

			if(drivechip == NULL) {
				if(find == 2)
					std::cout << "Open door." << std::endl;
				else
					std::cout << "Ram door." << std::endl;
				return 6;
			}
			else {
				//Is it facing us?
				if(find == 2) {
					std::string opensesame("OS!");
					drivechip->writecom(opensesame);
					return 4;
				}
				//else ram
				else {
					std::string ramcommand("MV 157 400");
					runcom(ramcommand);
					return 4;
				}
			}
		}
	}
	execcom();
	//else
	//	execcom(overridecom);

	return 0;
}

void drive_man::straighten(int set_check) {	
	if(set_check < 4)
		return;

	turnamount = set_check;
	overridemode = 8;
}

void drive_man::SetOverride(int set_override) {
	overridemode = set_override;
}

void drive_man::execcom() {
	if(drivechip != NULL)	 //Checking here so we can do simulations
		drivechip->writecom(currentpath);

	if(currentpath[0] == 'M' && currentpath[1] == 'V') {
		int drivedir = std::stoi(currentpath.substr(3,7));
		drivedir += ((robot->rot - 1.57)* 100);

		if(abs(drivedir - 471) < 5 || abs(drivedir - 157) < 5)
			 (abs(drivedir - 157) < 5) ? estimv.y += drivespeed : estimv.y -= drivespeed;
		else if (abs(drivedir) < 5 || abs(drivedir - 314) < 5)
			abs(drivedir < 5) ? estimv.x += drivespeed : estimv.x -= drivespeed;
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
		int drivedir = std::stoi(setstring.substr(3,7));
		drivedir += ((robot->rot - 1.57)* 100);

		if(abs(drivedir - 471) < 5 || abs(drivedir - 157) < 5)
			 (abs(drivedir - 157) < 5) ? estimv.y += drivespeed : estimv.y -= drivespeed;
		else if (abs(drivedir) < 5 || abs(drivedir - 314) < 5)
			abs(drivedir < 5) ? estimv.x += drivespeed : estimv.x -= drivespeed;
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
