/*****************************************************************
	* Author: Steven Radomski radomskist@yahoo.com
	*
	* Copyright (C) 2017 Steven Radomski
	* 
	* This file was part of the 2016 NIU robotics AI robot project
	* 
	* This code cannot be copied or used without the permission of
	* the author
	*
	*
*****************************************************************/
#ifndef _NAI_OPENGL
#define _NAI_OPENGL

#include "utils/ntime.hpp"
#include "Viewer/window.h"
#include "Infoproc/objects.hpp"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SDL2/SDL_opengl.h>


/*
	* OpenGL window for displaying the memory map
*/
class naigl : public basewin {
	public:
		naigl();
		~naigl();

		void draw();
		void makecurrent();
		void setplanes(const std::vector<obj_wall> &);
		void setpath(const std::vector<obj_point> );
		void setents(const std::vector<obj_cube> &);
		void GetKeys();

	protected:
		void shaderinit();
		SDL_GLContext nglcont;
		GLuint naishader;

		GLuint naivao;
		GLuint planebuffer[2];
		GLuint naivaopath;
		GLuint pathbuffer;

		GLuint naivaoobj;
		GLuint objbuffer[2];

		int pathlength;
		int planelength;
		int cubevertslen;

		//Math nerd stuff
		glm::mat4 proj;
		glm::mat4 view;
		float urotation, lrotation, xpos, ypos;
		GLint viewuni;
		bool linedepth;
		std::vector<GLfloat> planeverts;
		std::vector<GLfloat> planecolors;
};

#endif
