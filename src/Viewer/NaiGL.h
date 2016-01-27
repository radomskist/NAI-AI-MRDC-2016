#ifndef _NAI_OPENGL
#define _NAI_OPENGL

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class naigl {
	public:
		naigl();
		~naigl();

	private:
		SDL_Window* nglwin;
		SDL_GLContext nglcont;

};

#endif
