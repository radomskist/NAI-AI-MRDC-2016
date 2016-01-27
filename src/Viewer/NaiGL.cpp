#include "Viewer/NaiGL.h"

naigl::naigl() {
	width = 512;
	height = 424;
	//TODO: Make work with multiple windows
	win = SDL_CreateWindow( "Memory Map",
		0,
		0,
		width,
		height,
		SDL_WINDOW_OPENGL );

	nglcont = SDL_GL_CreateContext(win);
}


naigl::~naigl() {


}

