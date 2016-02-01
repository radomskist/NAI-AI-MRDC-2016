#include <iostream>

/*SDL headers for windows*/
#include <SDL2/SDL.h>
#include "Viewer/window.h"
#include "Viewer/NaiGL.h"
#include "Viewer/testwin.h"

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

	//This is temporary until we seperate the viewer from
	//Main program
	//Starting main loop

	bool areargs = argc > 1;
	std::string arg1;
	if(areargs)
		arg1 = argv[1];

	if((areargs && (arg1 == "-ct"))) {
		std::vector<nimg *> imageref;
		imageref = mainbrain.GetImages(BCCAM);
		testwin test_win(imageref[0]->width, imageref[0]->height, imageref[0]->depth);

		while(test_win.GetKeys()) {
			imageref = mainbrain.GetImages(BCCAM);

			if(!imageref.empty()) 
				test_win.setimg(imageref.front());
		}
	}
	else if((areargs == (arg1 == "-kin")) && mainbrain.KStatus()) {
		std::vector<nimg *> imageref;
		imageref = mainbrain.GetImages(KDEP);
		testwin test_win(imageref[0]->width, imageref[0]->height, imageref[0]->depth);

		while(test_win.GetKeys()) {
			imageref = mainbrain.GetImages(KDEP);

			if(!imageref.empty()) 
				test_win.setimg(imageref.front());
		}
	}
	else {
		naigl MemWin;
		world_map wmap = mainbrain.GetMap();
		wmap.gentest();	
		MemWin.addplanes(wmap.GetPlanes());
		std::vector<obj_cube> addcube;
		addcube.push_back(wmap.GetRobot());
		MemWin.addents(addcube);
		obj_point newpoint(2000,2000,0);
		MemWin.setpath(mainbrain.GetPath(newpoint));

		//TODO: limit to framerate
		while(MemWin.GetKeys()) {
			MemWin.draw();

		}

	}

	//Closing program
	return 0;
	}
