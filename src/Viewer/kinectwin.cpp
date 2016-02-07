#include "Viewer/kinectwin.h"
extern bool kimgdep;
extern bool kimgedge;
extern bool kimgline;
extern bool kimgplane;

kinwin::kinwin(unsigned int set_width, unsigned int set_height, unsigned int set_depth) : imgwin(set_width,set_height,set_depth) {
	slowcap = false;
	cap = true;
}

kinwin::~kinwin() {


}

bool kinwin::GetKeys() {
	SDL_Event event;
	//Checking for ESC key to close program
	bool go = true;
	while( SDL_PollEvent( &event ) )
		if(event.key.keysym.sym == SDLK_ESCAPE)
			go = false;

		/*camerapos*/
		else if(event.key.keysym.sym == SDLK_1 && event.type == SDL_KEYDOWN)
			kimgdep = !kimgdep;
		else if(event.key.keysym.sym == SDLK_2 && event.type == SDL_KEYDOWN)
			kimgedge = !kimgedge;
		else if(event.key.keysym.sym == SDLK_3 && event.type == SDL_KEYDOWN)
			kimgline = !kimgline;
		else if(event.key.keysym.sym == SDLK_4 && event.type == SDL_KEYDOWN)
			kimgplane = !kimgplane;
		else if(event.key.keysym.sym == SDLK_r && event.type == SDL_KEYDOWN)
			slowcap = !slowcap;
		else if(event.key.keysym.sym == SDLK_f && event.type == SDL_KEYDOWN)
			cap = true;

	return go;
}

void kinwin::setimg(nimg * updimg) {
	if(slowcap) {
		if (cap)
			cap = false;
		else
			return;

	}

	ctssurf = SDL_CreateRGBSurfaceFrom(updimg->data,
		updimg->width, updimg->height,
		8 * updimg->depth, //8 bits * 3 channels
		updimg->width * updimg->depth, //width * channels
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

