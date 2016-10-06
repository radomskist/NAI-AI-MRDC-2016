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
#ifndef NAI_STATE_FIRST_DOOR
#define NAI_STATE_FIRST_DOOR
#include "Infoproc/world_map.h"
#include "Infoproc/states/base_state.h"
#include "Infoproc/path_finding.h"
#include "Infoproc/states/go_inside.h"
class first_door : public base_state {

	public:
		first_door(const world_map*, path_finding&);
		~first_door();

		void intialize();
		int Process();
		void SetStat(std::string);
		base_state *endstate();
	protected:
		int mode;
		bool startingpos;
		bool infrontdoor;
		bool scanning;
		bool scanattempt;
		bool readytoopen;
		bool turndone;
		bool turning;
		bool qrread;
		bool readingqr;
		path_finding &pfind;
};

#endif
