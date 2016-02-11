#ifndef _NAI_MULTIWIN_BASE
#define _NAI_MULTIWIN_BASE

/*SDL headers for windows*/
#include <SDL2/SDL.h>
#include <iostream>
#include "Viewer/NaiGL.h"
#include "utils/nimg.h"
#include "Viewer/kinectwin.h"

class multiwin {
	public:
		multiwin();
		~multiwin();
		bool running();
		void GetKeys();

	private:
		//kinwin kdepth, kfreeze;
		//imgwin rgbwin;
		//naigl glwin;
};


#endif
