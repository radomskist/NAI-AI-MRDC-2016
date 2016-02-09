#include "Viewer/multiwin.h"
extern bool kimgdep;
extern bool kimgedge;
extern bool kimgline;
extern bool kimgplane;


multiwin::multiwin(unsigned int set_width, unsigned int set_height) : naigl() {
	/*adjusting pos of miniwindows*/
	rendcount = 3;

	rendcam = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
	SDL_SetWindowSize(win, set_width, set_height);
	glViewport(0,0,512,424);
	SDL_RenderClear(rendcam);
}


void multiwin::GetKeys() {
	SDL_Event event;

	//Checking for ESC key to close program

	std::stack<SDL_Event> noevent;

	while( SDL_PollEvent( &event ) )
		/*camerarot*/
		if(event.key.keysym.sym == SDLK_UP)
			urotation -= .1f;
		else if(event.key.keysym.sym == SDLK_DOWN)
			urotation += .1f;
		else if(event.key.keysym.sym == SDLK_LEFT)
			lrotation -=  .1f;
		else if(event.key.keysym.sym == SDLK_RIGHT)
			lrotation += .1f;

		/*camerapos*/
		else if(event.key.keysym.sym == SDLK_w)
			ypos -= 100.0f;
		else if(event.key.keysym.sym == SDLK_s)
			ypos += 100.0f;
		else if(event.key.keysym.sym == SDLK_a)
			xpos -= 100.0f;
		else if(event.key.keysym.sym == SDLK_d)
			xpos += 100.0f;

		/*Line depth test*/
		else if(event.key.keysym.sym == SDLK_f && event.type == SDL_KEYDOWN)
			linedepth = !linedepth;
		else if(event.key.keysym.sym == SDLK_1 && event.type == SDL_KEYDOWN)
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


void multiwin::setimg(std::vector<nimg*> &updimg) {
	
	SDL_Rect setpos;
	setpos.x = 0;
	setpos.y = 0;
	setpos.h = 512;
	setpos.w = 424;


	for(int i = 0; i < updimg.size(); i++) {
		int setrend = 0;

		if(updimg[i]->flags & (KFREEZE)) {
			setrend = 1;
			setpos.x = 0;
			setpos.y = 512;
			SDL_RenderSetViewport(rendcam, &setpos);
		}
		else if(updimg[i]->flags & (KDEP)) {
			setrend = 0;
			setpos.x = 0;
			setpos.y = 0;
			SDL_RenderSetViewport(rendcam, &setpos);
		}
		else if(updimg[i]->flags & (KRGB)) {
			setpos.x = 424;
			setpos.y = 512;
			setrend = 2;
			SDL_RenderSetViewport(rendcam, &setpos);
		}
		else
			continue;

		ctssurf = SDL_CreateRGBSurfaceFrom(updimg[i]->data,
			updimg[i]->width, updimg[i]->height,
			8 * updimg[i]->depth, //8 bits * 3 channels
			updimg[i]->width * updimg[i]->depth, //width * channels
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

	draw();
}


multiwin::~multiwin() {
	
}

