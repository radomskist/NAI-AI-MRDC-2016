#ifndef _NAI_MULTIWIN_BASE
#define _NAI_MULTIWIN_BASE

/*SDL headers for windows*/
#include <SDL2/SDL.h>
#include <iostream>
#include "Viewer/NaiGL.h"
#include "utils/nimg.h"

class multiwin : public naigl {
	public:
		multiwin(unsigned int, unsigned int);
		~multiwin();

		void GetKeys();
		void setimg(std::vector<nimg*> &);

	protected:
		int rendcount;
		SDL_Renderer *rendcam;
		SDL_Texture *textcam;
		SDL_Surface* ctssurf; //opencv to sdl surface

};

#endif
