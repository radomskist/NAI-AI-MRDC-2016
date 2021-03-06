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
#include "Infoproc/states/find_door.h"

first_door::first_door(const world_map* set_map, path_finding &set_pfind) : base_state(set_map), pfind(set_pfind) {
	mode = 0;
}
first_door::~first_door() {



}

void first_door::intialize() {
	const std::vector<obj_cube>& entities = wmap->GetEnts();

	//For testing
//	if(mode == 0) {
	if(mode == 2){
		mode++;
		pfind.gotopoint(obj_point(200,2200,50));
		return;
		}
//	else if(mode == 2){
	else if(mode == 0){
		//commlist = "SS a:157;f:d;o:1;d:l;";
		commlist = "SS a:0;f:d;o:1;";
		comred = true;
		mode = 2;
		return;
	}

	if(mode < 4)
		return;

	//Seeing if scan picked up door
	for(int i = 0; i < entities.size(); i++) {
		std::cout << entities[i].GetName() << std::endl;
		if(entities[i].GetName() != "door")
			continue;
		//Door found;

		float rotation = entities[i].rot;
		int gx = entities[i].pos.x;
		int gy = entities[i].pos.y;

		//seeing which way door is facing
		if(rotation < .05)
			gx += 400;
		else if(abs(rotation - 1.54) < .05)
			gx += 400;
		else if(abs(rotation - 3.14) < .05) 
			gx -= 400;
		else if(abs(rotation - 4.71) < .05)
			gx -= 400;
		else
			continue;

		if(pfind.gotopoint(obj_point(gx,gy,50)))
			mode++;
		else
			//failed do scan
			std::cout << "TODO: SCAN WHEN FAILED" << std::endl;

		return;
	}
	//Door not found
	pfind.gotopoint(obj_point(600,2600,50));
	mode = 1;
}

int first_door::Process() {
	if(mode < 5) {
		intialize();
		return 0;
	}

	//TODO makesure facing door
	if(mode == 5) {
		float turnangle = (6.24 - wmap->GetRobot()->rot) * 100;
		commlist = "RA+";
		commlist.append(std::to_string(turnangle));
		mode++;
		return 0;
	}
	if(mode == 7)
		sexit = 1;
}

base_state *first_door::endstate() {
	return new go_inside(wmap,pfind);
}

void first_door::SetStat(std::string set_state) {
	/*are we checking if a scan attempt is done?*/
	if(set_state[0] != '0') {
		mode++;
	}
}
