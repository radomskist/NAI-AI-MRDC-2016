#include "Infoproc/states/go_inside.h"

go_inside::go_inside(const world_map* set_map, path_finding& set_pfind) : base_state(set_map), pfind(set_pfind) {
	pfind.gotopoint(obj_point(1400,2200,50));

	inside = false;
}

go_inside::~go_inside() {

}

int go_inside::Process() {


}

void go_inside::SetStat(std::string set) {
	if(set[0] == '0') 
		return;

	if(!inside)
		inside = true;

	//TODO scan for dispenser

	//TODO: Search inside if no unused dispenser found
		//Basically just go to each corner of the inside scanning the area

	//TODO go to dispenser

}

base_state *go_inside::endstate() {
	//Operate dispenser
	return new operate_dis(wmap, pfind);
}
