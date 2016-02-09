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
	"out vec4 color;"
	"in VS_OUT"
	"{ vec4 scolor; } fs_in;"

	
	"void main()\n"
	"{\n"
	"gl_FragColor = fs_in.scolor;"
	"}"
};

const GLchar* wallshader = {"#version 330 core\n"
"layout (location = 0) in vec4 position;\n"
"layout (location = 1) in vec4 color;\n"

"uniform mat4 view;\n"

"out VS_OUT"
"{ vec4 scolor; } vs_out;"

"void main()\n"
	"{\n"
	"gl_Position.xyzw =  view * position.xyzw;"

    "vs_out.scolor = color;"	
	"}"
};


void naigl::GetKeys() {
	SDL_Event event;
	//Checking for ESC key to close program

	std::stack<SDL_Event> noevent;

	while( SDL_PollEvent( &event ) )
		/*camerarot*/
		if(event.key.keysym.sym == SDLK_UP)
			urotation -= .1f;
		else if(event.key.keysym.sym == SDLK_DOWN)
			urotation += .1f;
		else if(event.key.keysym.sym == SDLK_LEFT)
			lrotation -=  .1f;
		else if(event.key.keysym.sym == SDLK_RIGHT)
			lrotation += .1f;

		/*camerapos*/
		else if(event.key.keysym.sym == SDLK_w)
			ypos -= 100.0f;
		else if(event.key.keysym.sym == SDLK_s)
			ypos += 100.0f;
		else if(event.key.keysym.sym == SDLK_a)
			xpos -= 100.0f;
		else if(event.key.keysym.sym == SDLK_d)
			xpos += 100.0f;

		/*Line depth test*/
		else if(event.key.keysym.sym == SDLK_f && event.type == SDL_KEYDOWN)
			linedepth = !linedepth;
		else 
			noevent.push(event);

	ProcKeys(noevent);

}


naigl::naigl() {
	xpos = -600.0f;
	ypos = 0.0f;
	width = 640;
	height = 480;
	urotation = -0.59f;
	lrotation = 0.99f;
	linedepth = false;

	//TODO: Make sub window
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 ); 

	win = SDL_CreateWindow( "NAI",
		0,
		0,
		width,
		height,
		SDL_WINDOW_OPENGL );

	nglcont = SDL_GL_CreateContext(win);

	SDL_GL_MakeCurrent(win,
		nglcont);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); 
	glClearColor(.2,.2,.2,1);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(win);

	glewExperimental = GL_TRUE; 
	glewInit();

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK); TODO: renable later
	//glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
	

	/*Plane buffer*/
	glGenVertexArrays(2, &naivao);
	glBindVertexArray(naivao);
	glEnableVertexAttribArray(0);
	glGenBuffers(2, &planebuffer[0]);
	glBindBuffer(GL_ARRAY_BUFFER,
		planebuffer[0]);
	glBufferData(GL_ARRAY_BUFFER, int(sizeof(float)*3*4*1000), 0, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		NULL);

	//Creating color buffer
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER,
		planebuffer[1]);
	glBufferData(GL_ARRAY_BUFFER, int(sizeof(float)*3*4*1000), 0, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		NULL);

	//Setting ground buffer color
	for(int i = 0; i < 18; i++)
		planecolors.push_back(1.0f);

	/*pathbuffer*/
	glGenVertexArrays(1, &naivaopath);
	glBindVertexArray(naivaopath);
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &pathbuffer);
	glBindBuffer(GL_ARRAY_BUFFER,
		pathbuffer);
	glLineWidth(5.0f); //Sadly can't make as wide as NAI

	glVertexAttribPointer(0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		NULL);

	/*objs buffer*/
	glGenVertexArrays(2, &naivaoobj);
	glBindVertexArray(naivaoobj);
	glEnableVertexAttribArray(0);
	glGenBuffers(2, &objbuffer[0]);
	glBindBuffer(GL_ARRAY_BUFFER,
		objbuffer[0]);
	glBufferData(GL_ARRAY_BUFFER, int(sizeof(float)*3*8*1000), 0, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		NULL);

	//Creating color buffer
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER,
		objbuffer[1]);
	glBufferData(GL_ARRAY_BUFFER, int(sizeof(float)*3*8*200), 0, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		NULL);


	glViewport(0,0,width,height);

	//Creating our shaders
	shaderinit();

	viewuni = glGetUniformLocation(naishader, "view");
	proj = glm::perspective(1.57f, (float)width / (float)height, 1.5f, 100000.f); 

}

void naigl::setpath(std::vector<obj_point> set_newpath) {

	std::vector<float> newpath;
	glBindVertexArray(naivaopath);
	glBindBuffer(GL_ARRAY_BUFFER,
		pathbuffer);

	float pathheight = 50.0f;

	pathlength = set_newpath.size();
	for(int i = 0; i < pathlength; i++) {
		obj_point newpoint = set_newpath[i];
		newpath.push_back(newpoint.x);
		newpath.push_back(newpoint.y);
		newpath.push_back(pathheight);
	}
	pathlength = newpath.size();
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*pathlength, &newpath[0], GL_DYNAMIC_DRAW);
	draw();
}

inline void easypush(std::vector<float> &push, obj_point &point) {
	push.push_back(point.x);
	push.push_back(point.y);
	push.push_back(point.z);
}

void naigl::addents(std::vector<obj_cube> &add_objs) {

	std::vector<float> cubeverts;
	std::vector<float> cubecolorverts;

	for(int i = 0; i < add_objs.size(); i++){
		obj_cube newobj = add_objs[i];
		obj_point center = newobj.pos;
		float nh, nw;
		nh = newobj.height / 2;
		nw = newobj.width / 2;

		//Generating corners
		//TODO optimize
		obj_point corners[8];
		corners[0].x = center.x + nw;
		corners[0].y = center.y + nw;
		corners[0].z = center.z + nh;

		corners[1].x = center.x + nw;
		corners[1].y = center.y + nw;
		corners[1].z = center.z - nh;

		corners[2].x = center.x + nw;
		corners[2].y = center.y - nw;
		corners[2].z = center.z - nh;

		corners[3].x = center.x + nw;
		corners[3].y = center.y - nw;
		corners[3].z = center.z + nh;

		corners[4].x = center.x - nw;
		corners[4].y = center.y - nw;
		corners[4].z = center.z + nh;

		corners[5].x = center.x - nw;
		corners[5].y = center.y + nw;
		corners[5].z = center.z + nh;

		corners[6].x = center.x - nw;
		corners[6].y = center.y - nw;
		corners[6].z = center.z - nh;

		corners[7].x = center.x - nw;
		corners[7].y = center.y + nw;
		corners[7].z = center.z - nh;


		//Generating tris
		easypush(cubeverts, corners[0]);
		easypush(cubeverts, corners[1]);
		easypush(cubeverts, corners[2]);
		easypush(cubeverts, corners[0]);
		easypush(cubeverts, corners[3]);
		easypush(cubeverts, corners[2]);

		easypush(cubeverts, corners[0]);
		easypush(cubeverts, corners[3]);
		easypush(cubeverts, corners[4]);
		easypush(cubeverts, corners[0]);
		easypush(cubeverts, corners[5]);
		easypush(cubeverts, corners[4]);

		easypush(cubeverts, corners[0]);
		easypush(cubeverts, corners[1]);
		easypush(cubeverts, corners[5]);
		easypush(cubeverts, corners[1]);
		easypush(cubeverts, corners[7]);
		easypush(cubeverts, corners[5]);

		easypush(cubeverts, corners[1]);
		easypush(cubeverts, corners[2]);
		easypush(cubeverts, corners[6]);
		easypush(cubeverts, corners[1]);
		easypush(cubeverts, corners[7]);
		easypush(cubeverts, corners[6]);

		easypush(cubeverts, corners[5]);
		easypush(cubeverts, corners[7]);
		easypush(cubeverts, corners[6]);
		easypush(cubeverts, corners[5]);
		easypush(cubeverts, corners[4]);
		easypush(cubeverts, corners[6]);

		easypush(cubeverts, corners[3]);
		easypush(cubeverts, corners[4]);
		easypush(cubeverts, corners[6]);
		easypush(cubeverts, corners[3]);
		easypush(cubeverts, corners[2]);
		easypush(cubeverts, corners[6]);

		for(int j = 0; j < 30; j++) {
			cubecolorverts.push_back(add_objs[i].color[0]);
			cubecolorverts.push_back(add_objs[i].color[1]);
			cubecolorverts.push_back(add_objs[i].color[2]);
		}
	}

	glBindVertexArray(naivaoobj);
	glBindBuffer(GL_ARRAY_BUFFER,
		objbuffer[0]);
	cubevertslen = cubeverts.size();
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*cubevertslen, &cubeverts[0]);

	glBindBuffer(GL_ARRAY_BUFFER,
		objbuffer[1]);
	glBufferSubData(GL_ARRAY_BUFFER,0, sizeof(float)*cubevertslen, &cubecolorverts[0]);

	draw();
}


void naigl::addplanes(std::vector<obj_plane> &add_newplane) {
	for(int i = 0; i < add_newplane.size(); i++){
	
		obj_plane newplane = add_newplane[i];

		for(int i = 0; i < 3; i++){
			planeverts.push_back(newplane.p[i].x);
			planeverts.push_back(newplane.p[i].y);
			planeverts.push_back(newplane.p[i].z);
		}

		for(int i = 1; i < 4; i++){
			planeverts.push_back(newplane.p[i].x);
			planeverts.push_back(newplane.p[i].y);
			planeverts.push_back(newplane.p[i].z);
		}

		//Putting our plane colors in
		
		float setcolor = 1 - (newplane.slopey / 2);

		for(int i = 0; i < 18; i++)
			planecolors.push_back(setcolor);
	}
	
	glBindVertexArray(naivao);
	glBindBuffer(GL_ARRAY_BUFFER,
		planebuffer[0]);
	glBufferSubData(GL_ARRAY_BUFFER,0, sizeof(ground), ground);
	glBufferSubData(GL_ARRAY_BUFFER,sizeof(ground), sizeof(float)*planeverts.size(), &planeverts[0]);
	glBindBuffer(GL_ARRAY_BUFFER,
		planebuffer[1]);
	glBufferSubData(GL_ARRAY_BUFFER,0, sizeof(float)*planecolors.size(), &planecolors[0]);
	draw();
}	

void naigl::draw() {
	view = glm::rotate(glm::mat4(1), urotation, glm::vec3(1.0f,0.0f,0.0f));
	view = glm::rotate(view, lrotation, glm::vec3(0.0f,0.0f,1.0f));
	view = glm::translate(view, glm::vec3(ypos, xpos, -1500.0f));
	view = proj * view;
	
	glUniformMatrix4fv(viewuni, 1, 	0,  glm::value_ptr(view));
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glBindVertexArray(naivao);
	glDrawArrays(GL_TRIANGLES,0, sizeof(ground) + sizeof(float)*planeverts.size());

	glBindVertexArray(naivaoobj);
	glDrawArrays(GL_TRIANGLES,0, sizeof(float)*cubevertslen);

	if(linedepth)
		glDisable(GL_DEPTH_TEST);
	// Lets you see through walls
	glBindVertexArray(naivaopath);
	glDrawArrays(GL_LINE_STRIP,0, pathlength/3);

	if(linedepth)
		glEnable(GL_DEPTH_TEST);

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
		&wallshader,
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
