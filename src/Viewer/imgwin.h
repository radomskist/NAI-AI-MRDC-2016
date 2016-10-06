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
#ifndef _NAI_OPENCVWINDOWTEST
#define _NAI_OPENCVWINDOWTEST

/*SDL headers for windows*/
#include "Viewer/window.h"
#include "utils/nimg.h"

class imgwin : public basewin {
	public:
		imgwin(unsigned int, unsigned int, unsigned int);
		~imgwin();
		virtual void setimg(nimg *);

	protected:
		SDL_Renderer *rendcam;
		SDL_Texture *textcam;
		SDL_Surface* ctssurf; //opencv to sdl surface

};

#endif
