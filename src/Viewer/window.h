#ifndef _NAI_VIEWERTEST
#define _NAI_VIEWERTEST

/*SDL headers for windows*/
#include <SDL2/SDL.h>
#include <iostream>

class testwin {
	public:
		testwin();
		~testwin();
		void setimg(unsigned char *);
		bool IsRunning();

	private:
		SDL_Window* win;

		/*Pointer to opengl win*/
		SDL_Renderer *rendcam; //List of renderers
		SDL_Texture *textcam;

		unsigned int width, height;
};

#endif
