#ifndef _NAI_OPENCVWINDOWTEST
#define _NAI_OPENCVWINDOWTEST

/*SDL headers for windows*/
#include "Viewer/window.h"
#include "utils/nimg.h"

class opencvwin : public basewin {
	public:
		opencvwin(unsigned int, unsigned int, unsigned int);
		~opencvwin();
		void setimg(nimg *);

	private:
		SDL_Renderer *rendcam;
		SDL_Texture *textcam;
		unsigned int width, height, depth;
		SDL_Surface* ctssurf; //opencv to sdl surface

};

#endif
