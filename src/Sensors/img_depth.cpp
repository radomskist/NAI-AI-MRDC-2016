#include "Sensors/img_depth.h"
using namespace std;

imgd::imgd() {
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
		int ret_int = ((cpixlist[i].y*gapx*2 + (i+gapx))*pixjumpx + (cpixlist[i].y + gapy) * pixjumpy /*Indent (removed for now) + ((cpixlist[i].y % 2) * ( pixjumpx / 2))*/);

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

	/*loading points*/
	for(int i = 0; i < checktot; i++) {
		cpixlist[i].value = depthbuff[imcheckpos[i]];
		}

	/*starting the scan*/
	for(int i = 0; i < 1; i++) { //TODO FIX THIS TO checktot
		unsigned char dist = depthbuff[imcheckpos[i]];

		if(dist == 0) {
			depthbuff[imcheckpos[i]] = 255;
			depthbuff[imcheckpos[i] + 1] = 0;
			depthbuff[imcheckpos[i] + 2] = 0;

			cpixlist[i].added = checkswitch;
			continue;
		}

		//if(i % pixwidth <= 3 || i /pixheight <= 3)
		//	continue;

		int countx = 1;
		int county = 1;
		float slopex = 0;
		float slopey = 0;		

		/*plane found*/
		std::vector<cpixstr> ppoints;

		if(cpixlist[i].added != checkswitch && recurscan(slopex, dist, i+1, countx, true, ppoints) && recurscan(slopey, dist, i+pixwidth, county, false, ppoints)) {
			bool planeexist = false;
;
			/*Scaling slope to the image instead of from the starting point*/
			slopex = (i % pixwidth)*slopex;
			slopey = (i / pixwidth)*slopey;

			/*Searching for a plane with a matching slope*/
			for(std::vector<cplane>::iterator j = cplanelist.begin(); j != cplanelist.end(); j++) 
				/*if we get to empty slope place this in it*/
				 if(j->added != checkswitch) {
					j->added = checkswitch;
					j->slopex = slopex;
					j->slopey = slopey;
					j->points = ppoints;

					planeexist = true;
					break;
				}
				else if (0.5f > fabs(j->slopex/slopex) && 0.5f > fabs(j->slopey/slopey)) {
					j->points = ppoints;

					planeexist = true;
					break;
				}

			/*generating a new plane*/				
			if(!planeexist) {
				cplane newc;
				newc.slopex = slopex;
				newc.slopey = slopey;
				newc.points = ppoints;

				/*setting planes color*/
				int seed = cplanelist.size() * 6;
				srand(seed);
				newc.r = rand()%(255-1)+1;
				srand(seed+1);
				newc.b = rand()%(255-1)+1;
				srand(seed+2);
				newc.g = rand()%(255-1)+1;
				cplanelist.push_back(newc);

				newc.added = checkswitch;
			}
		}
		else
			cpixlist[i].added = checkswitch;
	}
	
	//Redrawing the planes
	for(std::vector<cplane>::iterator j = cplanelist.begin(); j != cplanelist.end(); j++) {

		if(j->added == checkswitch)
			break;

		for(std::vector<cpixstr>::iterator k = j->points.begin(); k != j->points.end(); k++){

			depthbuff[imcheckpos[k->pos]] = j->r;
			depthbuff[imcheckpos[k->pos] + 1] = j->g;
			depthbuff[imcheckpos[k->pos] + 2] = j->b;
		}

		j->added = checkswitch;		
	}
	checkswitch = !checkswitch;

}



bool imgd::recurscan(float &slope, unsigned char origvalue, int pos, int &count, bool right, std::vector<cpixstr> &invec) {

	if(cpixlist[pos].added == checkswitch)
		return false;

	/*Start our recursion*/
	if(count == 1) {
		if(cpixlist[pos].value == 0)
			return false;

		slope = cpixlist[pos].value/origvalue;

		bool isplane = false;

		if(right)
			recurscan(slope, origvalue,pos+1, ++count, true, invec);
		else 
			recurscan(slope, origvalue,pos+pixwidth, ++count, false, invec);

		if(count > 3) {
			int nextr = 1;
			float newslope;
			recurscan(newslope, origvalue, pos+1, nextr, true, invec);
			invec.push_back(cpixlist[pos]);

			return true;
		}
	}		
	else {
		if(cpixlist[pos].value == 0)
			return false;

		float difference = slope - cpixlist[pos].value/(origvalue + slope * (count-1));

		if(fabs(difference) > 0.05f + count*(0.015f)) { //TODO should scale with slope
		//std::cout << right << "  " << difference << "  " << (int)origvalue << "  " << (int)cpixlist[pos].value << "  " << (int)count << std::endl;
			return false;
		}
		if(!right)
			std::cout << difference << "  " << 0.05f + count*(0.01f) << std::endl;

		slope = (slope*(count-1) + cpixlist[pos].value/origvalue)/(count);

		if(right && count % pixwidth < pixwidth - 3) {
			recurscan(slope, origvalue,pos+1, ++count, true, invec);
			cpixlist[pos].added = checkswitch;
			invec.push_back(cpixlist[pos]);
			return true;
		}
		else if (!right && (pos+pixwidth) / pixwidth < pixheight - 3) {
			/*check right first*/
			int nextr = 1;
			float newslope = 0;
			if(!recurscan(newslope, origvalue, pos+1, nextr, true, invec)) /*making sure isn't a hair thin*/
				return true;

			recurscan(slope, origvalue, pos+pixwidth, ++count, false, invec);
			cpixlist[pos].added = checkswitch;
			invec.push_back(cpixlist[pos]);
			return true;
		}
		else
			return false;
		
	}
		
	//struct cpixstr { unsigned int pos; int x, y;bool added;}; /*structure for pixels being checked*/

	//cpixlist
	
	//TODO? Make number scale with pix dist and resolution

}


