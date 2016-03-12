#include "Infoproc/states/return_ball.h"

return_ball::return_ball(const world_map* set_map, path_finding& set_pfind) : base_state(set_map), pfind(set_pfind), robot(set_map->GetRobot()) {

	int xpos;
	if(robot->pos.x > 2800) {
		xpos = 1400;
		pfind.gotopoint(obj_point(xpos,robot->pos.y,50));
		mode = 1;
	}
	else {
		pfind.gotopoint(obj_point(1400,2200,50));
		mode = 2;
	}
}

return_ball::~return_ball() {


}

void return_ball::SetStat(std::string set) {
	if(set[0] == '0')
		return;
	
	//Going to entrance
	if(mode == 1)
		pfind.gotopoint(obj_point(1400,2200,50));
	//blowing this popsicle stand
	else if(mode == 2) 
		pfind.gotopoint(obj_point(600,2200,50));
	//Going to our ball bin
	else if(mode == 3)
		pfind.gotopoint(obj_point(600,200,50));
	else
		sexit = 1;

	mode++;
}


int return_ball::Process() {
	//Path finding is not reliable enough to find its way back on its own at this stage

}

base_state *return_ball::endstate() {
	//open bin state
	return new first_door(wmap,pfind);
}
