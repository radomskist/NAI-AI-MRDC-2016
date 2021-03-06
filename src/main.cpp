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
#include <iostream>

/*SDL headers for windows*/
#include <SDL2/SDL.h>
#include "Viewer/window.h"
#include "Viewer/NaiGL.h"
#include "Viewer/imgwin.h"
#include "Viewer/multiwin.h"
#include "Viewer/kinectwin.h"
#include <fstream>
#include <unistd.h>

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

		char *b = new char[1920*1080*4];
		imgref.data = new unsigned char[1920*1080*4];
		std::fstream fileopen ("f.bmp", std::ios::in | std::ios::binary);

		if(!fileopen.is_open())
			std::cout << "ERROR" << std::endl;

		fileopen.seekg (0, std::ios::end);
		int n = fileopen.tellg();
		fileopen.seekg (0, std::ios::beg);

		fileopen.seekg(0, std::ios::beg);
		fileopen.read(b, n);

		for(int i = 0; i < 1920*1080; i++) {
			imgref.data[i*4] = b[i*4  + 72];
			imgref.data[i*4+1] = b[i*4 + 1 + 72];
			imgref.data[i*4+2] = b[i*4 + 2 + 72];
		}
		imgrgb depthtest(105);
		depthtest.ProcessImg(imgref.data);
		test_win.setimg(depthtest.GetImg());

		while(test_win.running()) { 
			test_win.GetKeys();
		}
	}

	/*Memory window argument loops*/
	else if(areargs && (arg1 == "-mem")) {
		naigl MemWin;
		const world_map *wmap = mainbrain.GetMap();
		mainbrain.gentest();	
		MemWin.setplanes(wmap->GetPlanes());
		std::vector<obj_cube> addcube;
		obj_cube newbot = *(wmap->GetRobot());
		addcube.push_back(newbot);
		MemWin.setents(addcube);

		//TODO: limit to framerate
		while(MemWin.running()) {
			MemWin.GetKeys();
			MemWin.draw();
			MemWin.setpath(mainbrain.GetPfind().GetPath());
		}
	}
	else {
		sleep(2); //letting kinect warm up
		multiwin MainWin;
		const world_map *wmap = mainbrain.GetMap();
		unsigned int kmode = KDEP | KRGB | KFREEZE | BCCAM;

		mainbrain.gentest();
		std::vector<obj_cube> addcube;
		const obj_cube *newbot = wmap->GetRobot();
		addcube.push_back(*newbot);
		naigl *glwin = MainWin.GetGL();

		unsigned int mapversion = 3000; //random number so knows to update
		while(MainWin.running()) {
			mainbrain.tick();
			MainWin.GetKeys();
			MainWin.SetImg(mainbrain.GetImages(kmode));
			//TODO optimize
			if(mapversion != wmap->GetMapVersion()) {
				glwin->makecurrent();
				mapversion = wmap->GetMapVersion();
				glwin->setplanes(wmap->GetPlanes());
				//glwin->makecurrent();
			}
			glwin->setpath(mainbrain.GetPfind().GetPath());
			std::vector<obj_cube> cubelist = wmap->GetEnts();
			cubelist.push_back(*newbot);
			glwin->makecurrent();
			glwin->setents(cubelist);
			cubelist.pop_back();
			glwin->draw();
		}
		std::cout << "Exiting" << std::endl;
	}

	//Closing program
	return 0;
	}
