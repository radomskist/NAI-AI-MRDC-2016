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
#include "window.h"

basewin::basewin() {
	isrunning = true;
	fullscreen = false;
}

bool basewin::running() {
	return isrunning;
}

void basewin::GetKeys() {
	SDL_Event event;
	//Checking for ESC key to close program

	std::stack<SDL_Event> noevent;
	while( SDL_PollEvent( &event ) )
			noevent.push(event);
	ProcKeys(noevent);
}

void basewin::SetPos(unsigned int setx, unsigned int sety) {
	SDL_SetWindowPosition(win,setx,sety);
}

void basewin::ProcKeys(std::stack<SDL_Event> &events) {

	while(!events.empty()) {
		SDL_Event curevent = events.top();
		events.pop();
		if(curevent.key.keysym.sym == SDLK_ESCAPE)
			isrunning = false;
		else if(curevent.key.keysym.sym == SDLK_F11 && curevent.type == SDL_KEYDOWN) {
			fullscreen = !fullscreen;
			fullscreen ? SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN) : SDL_SetWindowFullscreen(win, 0);
		}
	}
}

basewin::~basewin() {
	SDL_SetWindowFullscreen(win, 0);

}

