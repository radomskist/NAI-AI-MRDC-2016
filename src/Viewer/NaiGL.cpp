#include "Viewer/NaiGL.h"

naigl::naigl() {
	width = 512;
	height = 424;
	//TODO: Make sub window

	win = SDL_CreateWindow( "Memory Map",
		0,
		0,
		width,
		height,
		SDL_WINDOW_OPENGL );

	nglcont = SDL_GL_CreateContext(win);

	SDL_GL_MakeCurrent(win,
		nglcont);

	glClearColor(.2,.2,.2,1);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(win);

	glewExperimental = GL_TRUE; 
	glewInit();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

	//Vertex array
	glGenVertexArrays(1, &naivao);
	glBindVertexArray(naivao);

	//Vertex buffer
	glGenBuffers(1, &planebuffer);
	glGenBuffers(1, &planebuffer);
		glBindBuffer(GL_ARRAY_BUFFER,
		planebuffer);

	glVertexAttribPointer(0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		NULL);

	glEnableVertexAttribArray(0);

}


void naigl::addplanes(std::vector<obj_plane> &) {
	
	
}

void naigl::draw() {
	glDrawArrays(GL_TRIANGLES,0,36);
	glFlush();
	SDL_GL_SwapWindow(win);

}
naigl::~naigl() {
	SDL_GL_DeleteContext(nglcont); 

}

