#ifndef _NAI_VIEWERTEST
#define _NAI_VIEWERTEST

/*SDL headers for windows*/
#include "Viewer/window.h"

class testwin : public basewin {
	public:
		testwin();
		~testwin();
		void setimg(unsigned char *);

	private:
		/*Pointer to opengl win*/
		SDL_Renderer *rendcam; //List of renderers
		SDL_Texture *textcam;

};

#endif
