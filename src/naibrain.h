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
#ifndef _NAIBRAIN_MAIN
#define _NAIBRAIN_MAIN
#include <stack>

#include "Devices/webcam.h"
#include "Devices/kinect_man.h"
#include "Devices/chipcomm.h"
#include "Infoproc/world_map.h"
#include "Infoproc/path_finding.h"
#include "Infoproc/localize.h"
#include "Infoproc/driveman.h"
#include "Infoproc/states/open_door.h"
#include "Infoproc/states/base_state.h"
#include "Infoproc/states/find_door.h"
#include "Infoproc/states/read_qr.h"
#include "Infoproc/states/argstate/scan_state.h"

#include "utils/nimg.h"
#include "utils/nexcep.hpp"

/*
	* Handles all the sensors and major AI classes
	*
	* Also should be the only class to make changes to the memory map
*/

class naibrain {
	public:
		naibrain();
		~naibrain();
		void gentest();
		std::vector<nimg*> &GetImages(unsigned int); /*get list of images from specified flags*/
		const world_map *GetMap(); /*Get world map*/
		path_finding &GetPfind(); /*Get path to a point from path find object*/
		void tick(); /*process every frame*/

		//TODO remove
		bool KStatus(); /*returns whether or not kinect is working*/

	private:
		unsigned char ballcolor,ballsaturation;

		bool usingwebcam;
		/*devices*/
		kinectman* kinect_manager; /*kinect manager object*/
 		std::vector<nimg*> Images; /*List of images from sensors*/
		nwebc *bcwebcam; /*webcam object*/

		/*ai objects*/
		world_map wmap; /*world map object*/
		path_finding pfind; /*path find object*/
		std::stack<base_state*> states; /*list of states*/
		drive_man driveman; /*driving manager*/
		localizer locsys; /*localization system*/
		
};

#endif
