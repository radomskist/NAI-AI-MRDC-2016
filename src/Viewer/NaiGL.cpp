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
	
	//Creating our shaders
	shaderinit();
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


void naigl::shaderinit() {
	/*
	TODO: Finish
	const GLchar const* shadertest = {"#version 330 core\n"
	"in vec4 position;\n"

	"out VS_OUT\n"
	"{"
	" vec4 color; \n"
	"} vs_out;"

	"uniform mat4 view;\n"
	"void main()\n"
		"{\n"

			"const vec4 colors[] = vec4[3](vec4(1.0,0.0,0.0,1.0), vec4(0.0,1.0,0.0,1.0), vec4(0.0, 0.0, 1.0, 1.0));\n"

			"gl_Position.xyzw =  view * position.xyzw;"

			"vs_out.color.x = sin(gl_Position.w / 6.25);\n"
			"vs_out.color.y = sin(gl_Position.w / 15.5);\n"
			"vs_out.color.z = sin(gl_Position.w / 39);"				
			"}"
	};

	//NOTE: TAKEN FROM THE SUPER BIBLE
	const GLchar const* fragmenttest = {"#version 330 core\n"
		"out vec4 color;\n"

		"in VS_OUT\n"
		"{ vec4 color;\n"
		"} fs_in;\n"

		"void main()\n"
		"{\n"
		"color = fs_in.color;"
		"}"
	};

	GLuint shaderobj;
	GLuint fragshadeobj;

	shaderobj = glCreateShader(GL_VERTEX_SHADER);  

	glShaderSource(shaderobj,
		1, 
		shadertest,
		NULL);

	glCompileShader(shaderobj);

	fragshadeobj = glCreateShader(GL_FRAGMENT_SHADER);  
		glShaderSource(fragshadeobj, 
		1,
		fragmenttest,
		NULL);

	glCompileShader(fragshadeobj); 


	GLint isCompiled = 0;
	glGetShaderiv(shaderobj, GL_COMPILE_STATUS, &isCompiled);

	if(isCompiled == GL_FALSE) {
		glGetShaderiv(shaderobj, GL_INFO_LOG_LENGTH, &isCompiled);
		GLchar* errormessage = new GLchar[isCompiled + 1];
	
		glGetShaderInfoLog(shaderobj, isCompiled, &isCompiled, errormessage);
	
		std::cout << "Vertex failed\n" << errormessage << std::endl << std::flush;
	}

	glGetShaderiv(fragshadeobj, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE) {
		glGetShaderiv(fragshadeobj, GL_INFO_LOG_LENGTH, &isCompiled);
		GLchar* errormessage = new GLchar[isCompiled + 1];
	
		glGetShaderInfoLog(fragshadeobj, isCompiled, &isCompiled, errormessage);
	
		std::cout << "Fragment failed\n" << errormessage << std::endl << std::flush;
	}

	naishader = glCreateProgram();
	glAttachShader(naishader, shaderobj);
	glAttachShader(naishader, fragshadeobj); 
	glLinkProgram(naishader); 

	glUseProgram(naishader); 
	*/

}
