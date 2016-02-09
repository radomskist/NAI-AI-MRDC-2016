#include "Viewer/kinectwin.h"
extern bool kimgdep;
extern bool kimgedge;
extern bool kimgline;
extern bool kimgplane;

kinwin::kinwin(unsigned int set_width, unsigned int set_height, unsigned int set_depth) : imgwin(set_width,set_height,set_depth) {
}

kinwin::~kinwin() {


}

void kinwin::GetKeys() {
	SDL_Event event;
	//Checking for ESC key to close program
	std::stack<SDL_Event> noevent;

	while( SDL_PollEvent( &event ) )
		/*camerapos*/
		if(event.key.keysym.sym == SDLK_1 && event.type == SDL_KEYDOWN)
			kimgdep = !kimgdep;
		else if(event.key.keysym.sym == SDLK_2 && event.type == SDL_KEYDOWN)
			kimgedge = !kimgedge;
		else if(event.key.keysym.sym == SDLK_3 && event.type == SDL_KEYDOWN)
			kimgline = !kimgline;
		else if(event.key.keysym.sym == SDLK_4 && event.type == SDL_KEYDOWN)
			kimgplane = !kimgplane;
		else
			noevent.push(event);

	ProcKeys(noevent);

}
