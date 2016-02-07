#ifndef _NAI_VIEWERBASE
#define _NAI_VIEWERBASE

/*SDL headers for windows*/
#include <SDL2/SDL.h>
#include <iostream>

class basewin {
	public:
		basewin();
		~basewin();

		virtual bool GetKeys();

	protected:
		unsigned int width,height,depth;
		SDL_Window* win;
};

#endif
