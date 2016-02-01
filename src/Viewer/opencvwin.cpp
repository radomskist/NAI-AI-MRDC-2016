#include "Viewer/opencvwin.h"

opencvwin::opencvwin(unsigned int set_width, unsigned int set_height, unsigned int set_depth) {
	ctssurf = 0;
	textcam = 0;
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

	rendcam = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
}

void opencvwin::setimg(nimg * updimg) {

	ctssurf = SDL_CreateRGBSurfaceFrom(updimg->data,
		updimg->width, updimg->height,
		24, //8 bits * 3 channels
		updimg->width*3, //width * channels
		0, 0, 0, 0);


	textcam = SDL_CreateTextureFromSurface(rendcam, 
		ctssurf);

	SDL_RenderClear(rendcam);
	SDL_RenderCopy(rendcam, 
		textcam,
		NULL,
		NULL);

	SDL_RenderPresent(rendcam);

}

opencvwin::~opencvwin() {

}

