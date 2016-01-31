#ifndef _NAI_VIEWERBASE
#define _NAI_VIEWERBASE

/*SDL headers for windows*/
#include <SDL2/SDL.h>
#include <iostream>

class basewin {
	public:
		basewin();
		~basewin();
		void setimg(unsigned char *);
		virtual bool GetKeys();

	protected:
		unsigned int width;
		unsigned int height;

		SDL_Window* win;
};

#endif
