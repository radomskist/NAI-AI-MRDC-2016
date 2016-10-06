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
#ifndef NAI_STATE_OPEN_DOOR
#define NAI_STATE_OPEN_DOOR
#include "Infoproc/world_map.h"
#include "Infoproc/path_finding.h"
#include "Infoproc/states/base_state.h"
/***********************


ARDUINO TEAM IS DOING DOOR OPENING
FOR THIS YEAR


*************************/

class open_door : public base_state {

	public:
		open_door(const world_map*, path_finding&);
		~open_door();

		int Process();
		void SetStat(std::string);

	protected:
		path_finding &pfind;
};

#endif
