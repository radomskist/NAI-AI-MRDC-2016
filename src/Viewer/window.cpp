#include "window.h"

testwin::testwin() {
	width = 512;
	height = 424;
	//Initializing the winodw
	win = SDL_CreateWindow("TEST", //name
		0, 0, //pos x,y
		width, height, //width, height
		0); //flags

	if(win == NULL) {
		throw "Window failed to create";
	}

	rendcam = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	/*intiailizing SDL texture*/
	textcam = SDL_CreateTexture(rendcam,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		width,
		height);
}

bool testwin::IsRunning() {
	SDL_Event event;
	//Checking for ESC key to close program
	while( SDL_PollEvent( &event ) )
		if(event.key.keysym.sym == SDLK_ESCAPE)
			return false;

	return true;
}

void testwin::setimg(unsigned char * updimg) {

	SDL_UpdateTexture(textcam,
		NULL,
		updimg,
		width*4);

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

