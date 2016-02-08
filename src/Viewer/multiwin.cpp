#include "Viewer/multiwin.h"

multiwin::multiwin() {

}

void multiwin::GetKeys() {
	SDL_Event event;
	//Checking for ESC key to close program

	std::stack<SDL_Event> noevent;
	while( SDL_PollEvent( &event ) )
			noevent.push(event);

	ProcKeys(noevent);
}


multiwin::~multiwin() {
	
}

