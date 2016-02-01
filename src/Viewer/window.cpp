#include "window.h"

basewin::basewin() {

}

bool basewin::GetKeys() {
	SDL_Event event;
	//Checking for ESC key to close program
	while( SDL_PollEvent( &event ) )
		if(event.key.keysym.sym == SDLK_ESCAPE)
			return false;

	return true;
}


basewin::~basewin() {
	//delete win; compiler bitches about this

}

