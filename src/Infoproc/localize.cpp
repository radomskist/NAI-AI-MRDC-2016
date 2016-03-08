#include "Infoproc/localize.h"

localizer::localizer(drive_man *set_dm, const obj_cube *set_r) : robot(set_r), 	driveman(set_dm) {
	front = true;
	left = true;
	right = true;
}

localizer::~localizer() {

}

/*run when dead end*/
void localizer::lost() {}

void localizer::SetComm(std::string set_str) {
	drivecomm = set_str;
}

void localizer::SetChecks(bool set_front, bool set_left, bool _setright) {
	//Checking if can move forward
	if((drivecomm)[0] == 'M' && (drivecomm)[1] == 'V') {
		if (!set_front) {
			driveman->SetObst(true);
			return;
		}
		else
			driveman->SetObst(false);
		//else if(!right)
		//	drivecomm = "RL 20!";
		//else
		//	drivecomm = "RR 20!";
	}

}




/*Approximating based on info*/
bool localizer::approximate(obj_point& dinc, float& dang){//drive incriment, drive angle

	//TODO still go through with kinect localize?
	//We didn't move a tall if this is the case
	if(!driveman->GetEst(dinc,dang))
		return false;

	dinc.x += robot->pos.x;
	dinc.y += robot->pos.y;
	dinc.z = 50;
	dang += robot->rot;

	if(dang > 6.28)
		dang -= 6.28;
	else if(dang < 0)
		dang += 6.28;

	return true;
}
