#include <Windows.h>
#include "SDL.h"

int main(int argc, char* argv[]) {
	if (!init(argc, argv)) return 1;
	if (!initKinect()) return 1;

	openGLSetup();

	// Main loop
	execute();


	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();

	return 0;
}