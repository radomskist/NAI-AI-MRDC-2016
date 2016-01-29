#include "Viewer/NaiGL.h"

const GLchar* fragmenttest = {"#version 330 core\n"
	"out vec4 color;\n"

	"in VS_OUT\n"
	"{ vec4 color;\n"
	"} fs_in;\n"

	"void main()\n"
	"{\n"
	"color = fs_in.color;"
	"}"
};
//NOTE: TAKEN FROM THE SUPER BIBLE

const GLchar* shadertest = {"#version 330 core\n"
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



naigl::naigl() {
	width = 512;
	height = 424;
	//TODO: Make sub window

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 ); 

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

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK); TODO: renable later
	glFrontFace(GL_CCW);

	//glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

	//Vertex array
	glGenVertexArrays(1, &naivao);
	glBindVertexArray(naivao);

	//Vertex buffer
	glEnableVertexAttribArray(0);

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
	
	glViewport(0,0,width,height);

	//Creating our shaders
	shaderinit();

	//Ground
	//4 feet * 11 squares * 100
	float test[18] = {
		4400.0f, 4400.0f, 0.0f,
		4400.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,

		0.0f, 4400.0f, 0.0f,
		4400.0f, 4400.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		};

	viewuni = glGetUniformLocation(naishader, "view");
	proj = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 10000.f); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(test), test, GL_DYNAMIC_DRAW);

}

void naigl::spin() {
	glClear(GL_COLOR_BUFFER_BIT);

	view = glm::translate(glm::mat4(1), glm::vec3(0.0f, -1000.0f, -2400.0f));
	view = glm::rotate(view, 40.0f, glm::vec3(1.0f,0.0f,0.0f));
	view = glm::rotate(view, -40.0f, glm::vec3(0.0f,0.0f,1.0f));

	view = proj * view;
	glUniformMatrix4fv(viewuni, 1, 	0,  glm::value_ptr(view));
	draw(); 
}

void naigl::addplanes(std::vector<obj_plane> &) {
	
	
}

void naigl::draw() {
	glDrawArrays(GL_TRIANGLES,0,sizeof(float)*18);
	glFlush();
	SDL_GL_SwapWindow(win);

}
naigl::~naigl() {
	SDL_GL_DeleteContext(nglcont); 

}


void naigl::shaderinit() {
	GLuint shaderobj;
	GLuint fragshadeobj;

	shaderobj = glCreateShader(GL_VERTEX_SHADER);  

	glShaderSource(shaderobj,
		1, 
		&shadertest,
		NULL);

	glCompileShader(shaderobj);
	fragshadeobj = glCreateShader(GL_FRAGMENT_SHADER);  
		glShaderSource(fragshadeobj, 
		1,
		&fragmenttest,
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
	

}
