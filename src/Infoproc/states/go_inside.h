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
#ifndef NAI_STATE_GO_INSIDE
#define NAI_STATE_GO_INSIDE
#include "Infoproc/world_map.h"
#include "Infoproc/states/operate_dispenser.h"
#include "Infoproc/path_finding.h"
#include "Infoproc/states/base_state.h"

class go_inside : public base_state {

	public:
		go_inside(const world_map*, path_finding&);
		~go_inside();

		int Process();
		void SetStat(std::string);
		base_state *endstate();
	protected:
		path_finding &pfind;
		int mode;
};

#endif
