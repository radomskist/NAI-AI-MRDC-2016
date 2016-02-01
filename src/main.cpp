#include <iostream>

/*SDL headers for windows*/
#include <SDL2/SDL.h>
#include "Viewer/window.h"
#include "Viewer/NaiGL.h"
#include "Viewer/testwin.h"
#include "Viewer/opencvwin.h"

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
		std::vector<nimg *> imageref;
		imageref = mainbrain.GetImages(BCCAM);
		opencvwin opencv_win(imageref[0]->width, imageref[0]->height, imageref[0]->depth);

		while(opencv_win.GetKeys()) {
			imageref = mainbrain.GetImages(BCCAM);

			if(!imageref.empty()) 
				opencv_win.setimg(imageref.front());
		}
	}

	/*Test kinect argument loop*/
	else if(mainbrain.KStatus() && (areargs == ((arg1 == "-kdep") || (arg1 == "-krgb")))) {
		int kmode;
		if(!areargs || (arg1 != "-krgb"))//default
			kmode = KDEP;
		else
			kmode = KRGB;

		std::vector<nimg *> imageref;
		imageref = mainbrain.GetImages(kmode);
		testwin test_win(imageref[0]->width, imageref[0]->height, imageref[0]->depth);

		while(test_win.GetKeys()) {
			imageref = mainbrain.GetImages(kmode);

			if(!imageref.empty()) 
				test_win.setimg(imageref.front());
		}
	}

	/*Memory window argument loops*/
	else {
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
		while(MemWin.GetKeys()) {
			MemWin.draw();

		}

	}

	//Closing program
	return 0;
	}
