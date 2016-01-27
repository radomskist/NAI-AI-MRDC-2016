#include <iostream>

/*SDL headers for windows*/
#include <SDL2/SDL.h>
#include "Viewer/window.h"
#include "Viewer/NaiGL.h"
#include "Viewer/testwin.h"

#include "naibrain.h"
int main()
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
	std::vector<unsigned char *> imageref;
	//If kinect is connected, work on kinect debugging
	if(mainbrain.KStatus()) {
		testwin test_win;

		while(test_win.IsRunning()) {
			imageref = mainbrain.GetImages(KDEP);

			if(!imageref.empty()) 
				test_win.setimg(imageref.front());
		}
	}
	//If no kinect load opengl fake map
	else {
		naigl MemWin;

		while(MemWin.IsRunning()) {


		}
		
	}

	//Closing program
	return 0;
	}
