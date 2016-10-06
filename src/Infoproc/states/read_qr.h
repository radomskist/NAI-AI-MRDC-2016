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
#ifndef NAI_STATE_READ_QR
#define NAI_STATE_READ_QR
#include "Infoproc/states/base_state.h"
#include "Devices/kinect_man.h"

class read_qr : public base_state {

	public:
		read_qr(world_map&, kinectman&);
		~read_qr();

		int Process();

	protected:
		kinectman &kinect_manager;
		world_map& nonconstwmap;
};

#endif
