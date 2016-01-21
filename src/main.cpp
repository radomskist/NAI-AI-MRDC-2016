#include <iostream>

/*SDL headers for windows*/
#include <SDL2/SDL.h>
#include "Viewer/window.h"
#include "naibrain.h"
int main()
	{
	/*TODO: The window needs to be moved to the viewer
	  *	which is a seperate program so we can see what the robot
	  *	is doing, seeing, thinking, etc while it's out in the field
	  *	but it is useful for now.
	*/
	testwin test_win;
	naibrain mainbrain;

	//Starting main loop
	std::vector<unsigned char *> imageref;
	while(test_win.IsRunning()) {
		imageref = mainbrain.GetImages(KDEP);

		if(!imageref.empty())
			test_win.setimg(imageref.front());
	}


	//Closing program
	return 0;
	}
