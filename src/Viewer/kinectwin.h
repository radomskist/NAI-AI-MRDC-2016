#ifndef _NAI_KINECTWINDOW
#define _NAI_KINECTWINDOW

/*SDL headers for windows*/
#include "Viewer/imgwin.h"
#include "utils/nimg.h"

class kinwin : public imgwin {
	public:
		kinwin(unsigned int, unsigned int, unsigned int);
		~kinwin();
		bool GetKeys();

	private:
		SDL_Renderer *rendcam;
		SDL_Texture *textcam;
		unsigned int width, height, depth;
		SDL_Surface* ctssurf; //opencv to sdl surface

};

#endif
