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
#ifndef _NAI_MULTIWIN_BASE
#define _NAI_MULTIWIN_BASE

/*SDL headers for windows*/
#include <SDL2/SDL.h>
#include <iostream>
#include "Viewer/NaiGL.h"
#include "utils/nimg.h"
#include "Viewer/kinectwin.h"

/*Specialized window to have multiple displays at once*/
class multiwin {
	public:
		multiwin();
		~multiwin();
		bool running();
		void SetImg(std::vector<nimg*>);
		void GetKeys();
		naigl *GetGL();

	private:
		kinwin kdepth, kfreeze;
		imgwin rgbwin;
		naigl glwin;
		bool isrunning;
};


#endif
