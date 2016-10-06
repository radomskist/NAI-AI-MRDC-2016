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
#include "Infoproc/states/base_state.h"
#include <iostream>
base_state::base_state() : wmap(NULL){
	comred = false;
	sexit = 0;
}
base_state::base_state(const world_map *set_map) : wmap(set_map) {
	comred = false;
	sexit = 0;
}

std::string &base_state::commands() {
	if(comred) {
		comred = false;
		return commlist;
	}
	else
		std::string();
}

int base_state::IsExit() { 
	return sexit; 
}
base_state *base_state::endstate() {
	return NULL;
}

base_state::~base_state() {



}
