#include "Sensors/img_depth.h"
using namespace std;

imgd::imgd() {
	/*Initializing image processing variables*/
	//TODO MAKE LOADABLE BY CONFIG FILE!
	configimg(4,512,424,0.95f);

}
imgd::~imgd() {


}
void imgd::configimg(int pixjumpx, unsigned int set_width, unsigned int set_height, float gapsize) {
	/*CONFIGURABLE VARIABLES*/
	height = set_height; /*resolution*/
	width = set_width;
	pixtot = height * width;

	/*figuring out how much space (in dots) should be between edge
	and dots*/
	pixjumpx *= 2;
	int gapx = (width - (width * gapsize)) / pixjumpx; /*X gap in pixels*/
	int gapy =  (height - (height * gapsize)) / pixjumpx; /*y gap in jumps*/

	/*Getting how many dots should be per row and collumn*/
	pixwidth = width / pixjumpx - gapx*2;
	pixheight = height / pixjumpx  - gapy*2;
	//pixpercol -= pixpercol / (gapx*2); /*taking spaces from gapx into account*/
	checktot = pixwidth * pixheight;

	/*adjusting jump pixels for calculations*/
	//pixjumpx *= 3; /*multiplying pixel distance by 3*/
	int pixjumpy = width * pixjumpx; /*moving the width over the amount of times we jump down*/

	imcheckpos = new unsigned int[pixtot];
	cpixlist = new cpixstr[pixtot];

	for(int i = 0; i < checktot; i++) {
		/*generating pixlist*/
		cpixlist[i].x = i % pixwidth;
		cpixlist[i].y = i / pixwidth;
		cpixlist[i].pos = i;
		/*Calculating positions of pixels to check*/
		int ret_int = ((cpixlist[i].y*gapx*2 + (i+gapx))*pixjumpx /*indent*/ + (cpixlist[i].y + gapy) * pixjumpy /*jump down each time*/ + ((cpixlist[i].y % 2) * ( pixjumpx / 2)));

		imcheckpos[i] = (checktot < ret_int) ? ret_int*3 : 0; /*add to array if in range, else set to 0*/
	}
}

float imgd::linearize(unsigned short lthis) {
	//TODO: load points into array ahead of time
	return (1.0 / (lthis * -0.0030711016 + 3.3309495161));
	}


void imgd::ProcessImg(unsigned char *depthbuff) {

	/*clearing list of points from previous scan*/
	for(std::vector<cplane>::iterator j = cplanelist.begin(); j != cplanelist.end(); j++) {
		j->points.clear();
		j->slopex = -999999;
		}

	/*loading points*/
	for(int i = 0; i < checktot; i++) {
		cpixlist[i].value = linearize(depthbuff[imcheckpos[i]]);
		}

	/*starting the scan*/
	for(int i = 0; i < checktot; i++) {
		depthbuff[imcheckpos[i]] = 255;
		depthbuff[imcheckpos[i] + 1] = 0;
		depthbuff[imcheckpos[i] + 2] = 0;

		int countx = 0;
		int county = 0;
		float slopex = 0;
		float slopey = 0;		

		/*plane found*/
		if(recurscan(depthbuff, &slopex, 0, &countx, true) && recurscan(depthbuff, &slopey, 0, &county, false)) {
			bool planeexist = false;

			/*Searching for a plane with a matching slope*/
			for(std::vector<cplane>::iterator j = cplanelist.begin(); j != cplanelist.end(); j++) 
				/*if we get to empty slope place this in it*/
				 if(j->slopex == -999999) {
					j->slopex = slopex;
					j->slopey = slopey;

					planeexist = true;
					break;
					}
				/*Comparing slope with slope in plane*/
				else {//if(abs(j->slopex - ) < 0.05f && abs(j->slopey - ) < 0.05f && abs(j->originx - ) && abs(j->originy - )) {

					planeexist = true;
					break;
					}
				
			if(!planeexist) {
				cplane newc;
				newc.slopex = slopex;
				newc.slopey = slopey;

				bool added = false;
				for(int k = 0; k < 3; k++)
					if(cpixlist[i+k].added != checkswitch)
						{
						cpixlist[i+k].added = checkswitch;
						newc.points.push_back(cpixlist[i+k]);
						added = true;
						}
				for(int k = 1; k < 3; k++)
					if(cpixlist[i+pixwidth*k].added != checkswitch)
						{
						cpixlist[i+pixwidth*k].added = checkswitch;
						newc.points.push_back(cpixlist[i+pixwidth*k]);
						added = true;
						}
				/*making sure we actually added something*/
				if(added) {
					int seed = cplanelist.size() * 6;
					srand(seed);
					newc.r = rand()%(255-1)+1;
					srand(seed+1);
					newc.b = rand()%(255-1)+1;
					srand(seed+2);
					newc.g = rand()%(255-1)+1;
					cplanelist.push_back(newc);
					}
				}
			}
		}

		//Redrawing the planes
		for(std::vector<cplane>::iterator j = cplanelist.begin(); j != cplanelist.end(); j++) {
			for(std::vector<cpixstr>::iterator k = j->points.begin(); k != j->points.end(); k++){
				depthbuff[imcheckpos[k->pos]] = j->r;
				depthbuff[imcheckpos[k->pos] + 1] = j->g;
				depthbuff[imcheckpos[k->pos] + 2] = j->b;
				}
			
			}
	checkswitch = !checkswitch;

}



bool imgd::recurscan(unsigned char *depthbuff, float *slope, int pos, int* count, bool right) {
	
	return false;
	}
