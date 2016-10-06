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
#ifndef NAI_STATE_ARG_SCAN
#define NAI_STATE_ARG_SCAN
#include <iostream>
#include <string>
#include <sstream>
#include "Infoproc/world_map.h"
#include "Devices/kinect_man.h"
#include "Infoproc/states/argstate/arg_base.h"

/*arguments
a:#; 
	angle, radian. Turns until it's facing this direction scanning all areas in between. 
	If it's the same as the current direction or isn't specified it does a 360 scan.
	Always positive (if you passive negative it'll be casted to positive)
d:(l or r); 
	direction, left or right (l or r). It turns this direction till it's facing the angle. If not specified defaults to right
o:(1 or 0); 
	One scan, true or false. Whether or not it should just scan the direction or scan the areas between (one for only the direction)
f:obj details; 
	d = QR code: look for door qr code
	s = QR code: look for dispenser qr code
	Find, TODO: commands b = ball, s = dispenser, b = score bin
*/

class scan_state : public arg_state {

	public:
		scan_state(const world_map*, std::string, kinectman&);
		~scan_state();

		int Process(); //Process information
		void SetStat(std::string);

	protected:
		inline void processscan();
		unsigned char shue,ssat;
		int failcount;
		bool liningup;
		bool close;
		int offset;
		bool init;
		bool once; //only scan the dir
		int mode;
		float angle; //angle to turn to relative to current rot in radians
		float direction; //true for right, false for left
		std::string scandir;
		std::string scantarg;
		std::string endcom;
		kinectman &kinect_manager;
};

#endif
