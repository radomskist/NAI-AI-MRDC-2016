#include "Sensors/img_depth.h"
using namespace std;

imgd::imgd() : 	iteration(3) {
	/*Initializing image processing variables*/
	//TODO MAKE LOADABLE BY CONFIG FILE!
	/*Has to be multiple of 2*/
	configimg(2,512,424,0.9f); /*range 50-95*/


}
imgd::~imgd() {


}

void imgd::configimg(int pixjumpx, unsigned int width, unsigned int height, float gapsize) {
	/*CONFIGURABLE VARIABLES*/

	/*figuring out how much space (in dots) should be between edge
	and dots*/
	unsigned int pixtot = width * height;
	float aspect = width / height;

	pixjumpx *= 2;
	int gapx = (width - (width * gapsize)) / pixjumpx; /*X gap in pixels*/
	int gapy =  gapx / aspect; /*y gap in jumps*/

	/*Getting how many dots should be per row and collumn*/
	pixwidth = width / pixjumpx - gapx*2;
	pixheight = (height / pixjumpx  - gapy*3);
	//pixwidth -= pixwidth / (gapx*8); /*taking spaces from gapx into account*/
	checktot = pixwidth * pixheight;

	/*adjusting jump pixels for calculations*/
	int pixjumpy = width * pixjumpx; /*moving the width over the amount of times we jump down*/

	imcheckpos = new unsigned int[pixtot];
	cpixlist = new cpixstr[pixtot];

	for(int i = 0; i < checktot; i++) {
		/*generating pixlist*/
		cpixlist[i].x = i % pixwidth;
		cpixlist[i].y = i / pixwidth;
		cpixlist[i].pos = i;
		/*Calculating positions of pixels to check*/
		int ret_int = ((cpixlist[i].y*gapx*2 + (i+gapx))*pixjumpx + (cpixlist[i].y + gapy) * pixjumpy); /*Indent (removed for now) + ((cpixlist[i].y % 2) * ( pixjumpx / 2))*/

		cpixlist[i].added = !checkswitch;

		if(checktot < ret_int) {
			imcheckpos[i] = ret_int*4;
		}
		else {
			checktot = i;
			return;
		}
	}
}

void imgd::ProcessImg(unsigned char *depthbuff) {
	twopixarray.clear();
	/*loading points*/
	for(int i = 0; i < checktot; i++) {
		cpixlist[i].value = depthbuff[imcheckpos[i]];
	}
	int checktest = pixwidth/2;
	for(int i = 0; i < checktest; i++) {
		

	}
	
	
	
}


