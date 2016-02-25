#include "Infoproc/localize.h"

localizer::localizer(drive_man &set_dm, const obj_cube *set_r) : robot(set_r), 	driveman(set_dm) {

}
localizer::~localizer() {

}

/*run when dead end*/
void localizer::lost() {}

/*Approximating based on info*/
bool localizer::approximate(obj_point& dinc, float& dang){//drive incriment, drive angle

	//TODO still go through with kinect localize?
	//We didn't move a tall if this is the case
	if(!driveman.GetEst(dinc,dang))
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
