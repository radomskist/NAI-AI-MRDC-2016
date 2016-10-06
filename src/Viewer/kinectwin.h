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
#ifndef _NAI_KINECTWINDOW
#define _NAI_KINECTWINDOW

/*SDL headers for windows*/
#include "Viewer/imgwin.h"
#include "utils/nimg.h"

/*Specifically for displaying kinect images*/
class kinwin : public imgwin {
	public:
		kinwin(unsigned int, unsigned int, unsigned int);
		~kinwin();
		void GetKeys();
};

#endif
