/*****************************************************************
	* Author: Steven Radomski radomskist@yahoo.com
	*
	* Copyright (C) 2017 Steven Radomski
	* 
	* This file was part of the 2016 NIU robotics AI robot project
	* 
	* This code cannot be copied or used without the permission of
	* the author
	*
	*
*****************************************************************/
#include "Infoproc/states/go_inside.h"
extern int disnum; //Crunch time caused this

go_inside::go_inside(const world_map* set_map, path_finding& set_pfind) : base_state(set_map), pfind(set_pfind) {
	pfind.gotopoint(obj_point(1400,2200,50));

	mode = 0;
}

go_inside::~go_inside() {

}

int go_inside::Process() {


}

void go_inside::SetStat(std::string set) {
	if(set[0] == '0')
		return;

	if(mode < 5) {
		if(disnum == 0) {
			if(mode == 0)
				pfind.gotopoint(obj_point(1400,1400,50));
			else if(mode == 1) {
				commlist = "SS a:157;f:s;o:1;d:l;";
				comred = true;
			}
			else {
				sexit = 1;
				disnum++;
			}

		}
		else if(disnum == 1) {
			if(mode == 0)
				pfind.gotopoint(obj_point(1400,3000,50));
			else if(mode == 1) {
				commlist = "SS a:157;f:s;o:1;";
				comred = true;
			}
			else {
				sexit = 1;
				disnum++;
			}
		}
		else if(disnum == 2) {
			if(mode == 0)
				pfind.gotopoint(obj_point(1400,3000,50));
			else if(mode == 1) {
				pfind.gotopoint(obj_point(3000,3000,50));
			}
			else if(mode == 2) {
				commlist = "SS a:0;f:s;o:1;";
				comred = true;
			}
			else {
				sexit = 1;
				disnum++;
			}
		}
		else if(disnum == 3) {
			if(mode == 0)
				pfind.gotopoint(obj_point(1400,1400,50));
			else if(mode == 1)
				pfind.gotopoint(obj_point(3000,1400,50));
			else if(mode == 2) {
				commlist = "SS a:0;f:s;o:1;";
				comred = true;
			}
			else {
				sexit = 1;
				disnum++;
			}
		}
	}
 
	mode++;
	return;
}

base_state *go_inside::endstate() {
	//Operate dispenser
	return new operate_dis(wmap, pfind);
}
