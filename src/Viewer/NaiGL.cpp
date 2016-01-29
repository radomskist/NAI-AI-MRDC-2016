#include "Viewer/NaiGL.h"
//Ground
//4 feet * 11 squares * 100
const float ground[18] = {
	4400.0f, 4400.0f, 0.0f,
	4400.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	0.0f, 4400.0f, 0.0f,
	4400.0f, 4400.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	};

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
	width = 640;
	height = 480;
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

	glEnable(GL_DEPTH_TEST);
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

	viewuni = glGetUniformLocation(naishader, "view");
	proj = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 10000.f); 
	glBufferData(GL_ARRAY_BUFFER, int(sizeof(float)*3*4*1000), 0, GL_DYNAMIC_DRAW);

}

void naigl::spin() {
	//view = glm::translate(glm::mat4(1), glm::vec3(0.0f, 1000.0f, 2400.0f));
	view = glm::rotate(glm::mat4(1), -0.79f , glm::vec3(1.0f,0.0f,0.0f));
	view = glm::rotate(view, 0.79f , glm::vec3(0.0f,0.0f,1.0f));
	view = glm::translate(view, glm::vec3(500.0f, 500.0f, -2000.0f));
	//view = glm::rotate(view, 2.36f, glm::vec3(0.0f,0.0f,1.0f));

	view = proj * view;
	glUniformMatrix4fv(viewuni, 1, 	0,  glm::value_ptr(view));
	draw(); 
}

void naigl::addplanes(std::vector<obj_plane> &add_newplane) {
	for(int i = 0; i < add_newplane.size(); i++){
	
		obj_plane newplane = add_newplane[i];
		//Triangle 1
		planeverts.push_back(newplane.x1.x);
		planeverts.push_back(newplane.x1.y);
		planeverts.push_back(newplane.x1.z);

		planeverts.push_back(newplane.y1.x);
		planeverts.push_back(newplane.y1.y);
		planeverts.push_back(newplane.y1.z);

		planeverts.push_back(newplane.y2.x);
		planeverts.push_back(newplane.y2.y);
		planeverts.push_back(newplane.y2.z);


		//Triangle 2
		planeverts.push_back(newplane.x1.x);
		planeverts.push_back(newplane.x1.y);
		planeverts.push_back(newplane.x1.z);

		planeverts.push_back(newplane.y2.x);
		planeverts.push_back(newplane.y2.y);
		planeverts.push_back(newplane.y2.z);

		planeverts.push_back(newplane.x2.x);
		planeverts.push_back(newplane.x2.y);
		planeverts.push_back(newplane.x2.z);
	}

	glBufferSubData(GL_ARRAY_BUFFER,0, sizeof(ground), ground);
	glBufferSubData(GL_ARRAY_BUFFER,sizeof(ground), sizeof(float)*planeverts.size(), &planeverts[0]);
	draw();
}	

void naigl::draw() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES,0,sizeof(ground) + sizeof(float)*planeverts.size());
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
