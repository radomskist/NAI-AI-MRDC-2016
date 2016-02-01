#ifndef _NAI_VIEWERTEST
#define _NAI_VIEWERTEST

/*SDL headers for windows*/
#include "Viewer/window.h"
#include "utils/nimg.h"

class testwin : public basewin {
	public:
		testwin(unsigned int, unsigned int, unsigned int);
		~testwin();
		void setimg(nimg *);

	private:
		SDL_Renderer *rendcam;
		SDL_Texture *textcam;
		unsigned int width, height, depth;

};

#endif
