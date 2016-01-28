#ifndef _NAI_OPENGL
#define _NAI_OPENGL

#include "Viewer/window.h"
#include "Memory/objects.hpp"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL_opengl.h>

class naigl : public basewin {
	public:
		naigl();
		~naigl();


		void draw();
		void addplanes(std::vector<obj_plane> &);
	private:
		void shaderinit();
		SDL_GLContext nglcont;
		GLuint naishader;

		GLuint naivao;

		GLuint planebuffer;
};

#endif
