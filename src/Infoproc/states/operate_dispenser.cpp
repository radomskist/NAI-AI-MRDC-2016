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
#include "Infoproc/world_map.h"
#include "Infoproc/states/operate_dispenser.h"

operate_dis::operate_dis(const world_map* set_map, path_finding& set_pfind) : base_state(set_map), pfind(set_pfind)  {
	sexit = 1;

}

operate_dis::~operate_dis() { }

//Process information
int operate_dis::Process() {

	//Line up

	//Drive foward

	//reallign

	//Loops
		//turn

		//Check ball color
	//when none end

}

base_state *operate_dis::endstate(){ //Which state to go into next? 
	return new return_ball(wmap, pfind);
}

void operate_dis::SetStat(std::string) {

}

