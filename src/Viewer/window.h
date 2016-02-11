#ifndef _NAI_VIEWERBASE
#define _NAI_VIEWERBASE

/*SDL headers for windows*/
#include <SDL2/SDL.h>
#include <iostream>
#include <stack>

class basewin {
	public:
		basewin();
		~basewin();

		void SetPos(unsigned int, unsigned int);
		void ProcKeys(std::stack<SDL_Event> &);
		virtual void GetKeys();
		bool running();
	protected:
		unsigned int width,height,depth;
		SDL_Window* win;
		bool isrunning;
		bool fullscreen;
	private:

};

#endif
