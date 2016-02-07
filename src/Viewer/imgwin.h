#ifndef _NAI_OPENCVWINDOWTEST
#define _NAI_OPENCVWINDOWTEST

/*SDL headers for windows*/
#include "Viewer/window.h"
#include "utils/nimg.h"

class imgwin : public basewin {
	public:
		imgwin(unsigned int, unsigned int, unsigned int);
		~imgwin();
		virtual void setimg(nimg *);

	protected:
		SDL_Renderer *rendcam;
		SDL_Texture *textcam;
		SDL_Surface* ctssurf; //opencv to sdl surface

};

#endif
