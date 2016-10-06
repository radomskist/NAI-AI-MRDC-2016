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
#ifndef NAI_LOCALIZER
#define NAI_LOCALIZER
#include "Infoproc/world_map.h"
#include "Infoproc/driveman.h"
#include <string>

/*
	* Attempt to manage all localization data about the project
	* Will probably be replaced in the future
*/
class localizer {
	public:
		localizer(drive_man *, const obj_cube *);
		~localizer();

		/*Approximating based on info*/
		bool approximate(obj_point&, float&);
		void SetComm(std::string set_str);

		/*Dead end?*/
		void SetChecks(bool,bool,bool);

		/*run when dead end*/
		void lost();
		
	private:
		const obj_cube *robot;
		drive_man *driveman;
		bool front,right,left; //Can we go in these directions
		std::string drivecomm;
		

};

#endif
