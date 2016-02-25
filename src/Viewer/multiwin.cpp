#include "Viewer/multiwin.h"
/*ignore this pls*/
extern bool kimgdep;
extern bool kimgedge;
extern bool kimgline;
extern bool kimgplane;


multiwin::multiwin() : kdepth(512,424,3), kfreeze(512,424,3), rgbwin(512,424,3){
	isrunning = true;

	kdepth.SetPos(0,0);
	kfreeze.SetPos(512,0);
	rgbwin.SetPos(512,424);
	glwin.SetPos(0,424);
}

void multiwin::SetImg(std::vector<nimg*> set_img) {
	for(int i = 0; i < set_img.size(); i++) {
		if(set_img[i]->flags & KFREEZE)
			kfreeze.setimg(set_img[i]);

		if(set_img[i]->flags & KDEP) 
			kdepth.setimg(set_img[i]);
		else if (set_img[i]->flags & KRGB) 
			rgbwin.setimg(set_img[i]);
	}

	glwin.draw();
}

naigl *multiwin::GetGL() {
	return &glwin;
}
void multiwin::GetKeys() {
	SDL_Event event;
	//Checking for ESC key to close program

	while( SDL_PollEvent( &event ) )
		/*camerapos*/
		if(event.key.keysym.sym == SDLK_1 && event.type == SDL_KEYDOWN)
			kimgdep = !kimgdep;
		else if(event.key.keysym.sym == SDLK_2 && event.type == SDL_KEYDOWN)
			kimgedge = !kimgedge;
		else if(event.key.keysym.sym == SDLK_3 && event.type == SDL_KEYDOWN)
			kimgline = !kimgline;
		else if(event.key.keysym.sym == SDLK_4 && event.type == SDL_KEYDOWN)
			kimgplane = !kimgplane;
		else if(event.key.keysym.sym == SDLK_ESCAPE && event.type == SDL_KEYDOWN)
			isrunning = false;
		else if(event.key.keysym.sym == SDLK_F11 && event.type == SDL_KEYDOWN) {
				kdepth.SetPos(0,0);
				kfreeze.SetPos(512,0);
				rgbwin.SetPos(512,424);
				glwin.SetPos(0,424);
		}
	glwin.GetKeys();

}

bool multiwin::running() {
	return isrunning;
}

multiwin::~multiwin() {
	
}

