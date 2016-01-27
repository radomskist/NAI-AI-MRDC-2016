#include "Viewer/NaiGL.h"

naigl::naigl() {

	//TODO: Make work with multiple windows
	nglwin = SDL_CreateWindow( "Memory Map",
		0,
		0,
		512,
		424,
		SDL_WINDOW_OPENGL );

	nglcont = SDL_GL_CreateContext(nglwin);
}

naigl::~naigl() {


}

