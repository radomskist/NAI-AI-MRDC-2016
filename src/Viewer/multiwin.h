#ifndef _NAI_MULTIWIN_BASE
#define _NAI_MULTIWIN_BASE

/*SDL headers for windows*/
#include <SDL2/SDL.h>
#include <iostream>
#include "Viewer/window.h"

class multiwin : public basewin {
	public:
		multiwin();
		~multiwin();

		void GetKeys();

	protected:
};

#endif
