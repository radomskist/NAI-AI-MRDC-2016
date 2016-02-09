#include <iostream>

/*SDL headers for windows*/
#include <SDL2/SDL.h>
#include "Viewer/window.h"
#include "Viewer/NaiGL.h"
#include "Viewer/imgwin.h"
#include "Viewer/multiwin.h"
#include "Viewer/kinectwin.h"
#include <fstream>

#include "naibrain.h"
int main(int argc, char** argv)
	{
	/*TODO: The window needs to be moved to the viewer
	  *	which is a seperate program so we can see what the robot
	  *	is doing, seeing, thinking, etc while it's out in the field
	  *	but it is useful for now.
	*/

    SDL_Init( SDL_INIT_EVERYTHING );

	naibrain mainbrain;

	bool areargs = argc > 1;
	std::string arg1;
	if(areargs)
		arg1 = argv[1];


	//Starting main loop

	/*Test camera argument loop*/
	if((areargs && (arg1 == "-ct"))) {
		std::vector<nimg*> imageref;
		imageref = mainbrain.GetImages(BCCAM);

		if(imageref.empty()) {
			std::cout << "No webcam" << std::endl;
			return -12;
		}

		imgwin opencv_win(imageref[0]->width, imageref[0]->height, imageref[0]->depth);

		while(opencv_win.running()) {
			imageref = mainbrain.GetImages(BCCAM);
			opencv_win.GetKeys();

			if(!imageref.empty()) 
				opencv_win.setimg(imageref.front());

		}
	}

	/*Test kinect argument loop*/
	else if(mainbrain.KStatus() && areargs && (arg1 == "-kdep") || (arg1 == "-krgb")) {
		int kmode;
		if(!areargs || (arg1 != "-krgb"))//default
			kmode = KDEP;
		else
			kmode = KRGB;

		std::vector<nimg*> imageref;
		imageref = mainbrain.GetImages(kmode);
		kinwin test_win(imageref[0]->width, imageref[0]->height, imageref[0]->depth);

		while(test_win.running()) {
			test_win.GetKeys();
			imageref = mainbrain.GetImages(kmode);

			if(!imageref.empty()) 
				test_win.setimg(imageref.front());
		}
	}

	/*kinect image process test*/
	else if(areargs && (arg1 == "-kimg")) {

		nimg imgref;
		imgref.width = 512;
		imgref.height = 424;
		imgref.depth = 3;

		imgwin test_win(imgref.width, imgref.height, imgref.depth);

		char *b = new char[512*424*3];
		imgref.data = new unsigned char[512*424*3];
		std::fstream fileopen ("f.bmp", std::ios::in | std::ios::binary);

		if(!fileopen.is_open())
			std::cout << "ERROR" << std::endl;

		fileopen.seekg (0, std::ios::end);
		int n = fileopen.tellg();
		fileopen.seekg (0, std::ios::beg);

		fileopen.seekg(0, std::ios::beg);
		fileopen.read(b, n);

		for(int i = 0; i < 512*424; i++) {
			imgref.data[i*3] = b[i*3+54];
			imgref.data[i*3+1] = b[i*3+54];
			imgref.data[i*3+2] = b[i*3+54];
			//imgref.data[i*3 + 3] = b[i*4 + 72];
		}
		imgd depthtest;

		depthtest.ProcessImg(imgref.data);
		test_win.setimg(&imgref);

		while(test_win.running()) { 
			test_win.GetKeys();
		}
	}

	/*Memory window argument loops*/
	else if(areargs && (arg1 == "-mem")) {
		naigl MemWin;
		world_map wmap = mainbrain.GetMap();
		wmap.gentest();	
		MemWin.addplanes(wmap.GetPlanes());
		std::vector<obj_cube> addcube;
		addcube.push_back(wmap.GetRobot());
		MemWin.addents(addcube);
		obj_point newpoint(2000,2000,0);
		MemWin.setpath(mainbrain.GetPfind().gotopoint(newpoint));

		//TODO: limit to framerate
		while(MemWin.running()) {
			MemWin.GetKeys();
			MemWin.draw();
		}
	}
	else {
		multiwin MainWin(1024, 760);
		world_map wmap = mainbrain.GetMap();
		unsigned int kmode = KDEP & KRGB & KFREEZE;

		/*TODO REMOVE*/
		wmap.gentest();	
		MainWin.addplanes(wmap.GetPlanes());
		std::vector<obj_cube> addcube;
		addcube.push_back(wmap.GetRobot());
		MainWin.addents(addcube);
		obj_point newpoint(2000,2000,0);
		MainWin.setpath(mainbrain.GetPfind().gotopoint(newpoint));

		while(MainWin.running()) {
			MainWin.GetKeys();
			MainWin.setimg(mainbrain.GetImages(kmode));
		}
	}

	//Closing program
	return 0;
	}
