#include "Viewer/testwin.h"

testwin::testwin(unsigned int set_width, unsigned int set_height, unsigned int set_depth) {
	width = set_width;
	height = set_height;
	depth = set_depth;
	//Initializing the winodw
	win = SDL_CreateWindow("TEST", //name
		0, 0, //pos x,y
		width, height, //width, height
		0); //flags

	if(win == NULL) {
		throw "Window failed to create";
	}

	//SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);

	rendcam = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	/*intiailizing SDL texture*/
	textcam = SDL_CreateTexture(rendcam,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		width,
		height);

}

void testwin::setimg(nimg * updimg) {

	/*Creating a texture out of the image so SDL can render it*/
	SDL_UpdateTexture(textcam,
		NULL,
		updimg->data,
		width*depth);

	SDL_UnlockTexture(textcam);
	SDL_RenderClear(rendcam);
	SDL_RenderCopy(rendcam, 
		textcam,
		NULL,
		NULL);

	SDL_RenderPresent(rendcam);

}

testwin::~testwin() {


}

