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
#ifndef NAI_STATE_OP_DIS
#define NAI_STATE_OP_DIS
#include "Infoproc/world_map.h"
#include "Infoproc/path_finding.h"
#include "Infoproc/states/base_state.h"
#include "Infoproc/states/return_ball.h"

class operate_dis : public base_state {

	public:
		operate_dis(const world_map*, path_finding&);
		~operate_dis();

		int Process(); //Process information
		base_state *endstate(); //Which state to go into next?
		void SetStat(std::string);

	protected:
		path_finding &pfind;
};

#endif
