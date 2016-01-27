#ifndef _NAI_OPENGL
#define _NAI_OPENGL

#include "Viewer/window.h"
class naigl : public basewin {
	public:
		naigl();
		~naigl();

	private:
		SDL_GLContext nglcont;

};

#endif
