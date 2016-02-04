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

/*CONFIGURABLE VARIABLES*/
void imgd::configimg(int pixjumpx, unsigned int width, unsigned int height, float gapsize) {


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
		cpixlist[i].x = i % pixwidth + 1;
		cpixlist[i].y = i / pixwidth + 1;
		cpixlist[i].pos = i;
		/*Calculating positions of pixels to check*/
		int ret_int = ((cpixlist[i].y*gapx*2 + (i+gapx))*pixjumpx + (cpixlist[i].y + gapy) * pixjumpy); /*Indent (removed for now) + ((cpixlist[i].y % 2) * ( pixjumpx / 2))*/

		if(checktot < ret_int) 
			imcheckpos[i] = ret_int*4;
		else {
			checktot = i;
			break;
		}
	}

	/*doing twopix resolution*/
	tptot = checktot/2;
	tpwidth = pixwidth/2;
	twopixarray = new ctwopix[tptot];

	for(int i = 0; i < tptot; i++)
		twopixarray[i].added = !checkswitch;

	if(tpwidth % 2 != 0)
		tpwidth--;
}

/*checking two points*/
int imgd::checktwopoints(unsigned int first, unsigned int second) {
	float testslope = twopixarray[second].slope - twopixarray[first].slope;
	if(testslope > 0.5f) //Tolerance
		return 1;

	//Seeing if foreground or background
	if(twopixarray[second].slope > twopixarray[first].slope)
		return 2;
	
	return 0;
}

/*Two pixel slope check suggested by Joel*/
void imgd::ProcessImg(unsigned char *depthbuff) {
	cplanelist.clear();

	/*loading points*/
	for(int i = 0; i < checktot; i++) 
		cpixlist[i].value = depthbuff[imcheckpos[i]];

	/*calling the process function on each point*/
	for(int i = 0; i < tptot; i++) {
		cplane newc;

		processpoint(i,newc);

		if(newc.points.size() > 5); //What's considered a plane(more than this many pixels)
			cplanelist.push_back(newc);
	}

	//Redrawing the planes
	for(std::vector<cplane>::iterator j = cplanelist.begin(); j != cplanelist.end(); j++) {
		if(j->added != checkswitch)
			break;

		for(std::vector<cpixstr>::iterator k = j->points.begin(); k != j->points.end(); k++){

			depthbuff[imcheckpos[k->pos]] = j->r;
			depthbuff[imcheckpos[k->pos] + 1] = j->g;
			depthbuff[imcheckpos[k->pos] + 2] = j->b;
		}
	}

	checkswitch = !checkswitch;	
}


void imgd::addtoplane(ctwopix &newpoints, ctwopix &newpointstwo) {
	bool planeexist = false;
	newpoints.added = checkswitch;
	newpointstwo.added = checkswitch;

	/*Searching for a plane with a matching slope*/
	for(std::vector<cplane>::iterator j = cplanelist.begin(); j != cplanelist.end(); j++) 
		/*if we get to empty slope place this in it*/
		 if(j->added != checkswitch) {
			j->added = checkswitch;
			j->slopex = newpoints.slope;

			j->points.clear();
			j->points.push_back(newpoints.points[0]);
			j->points.push_back(newpoints.points[1]);
			j->points.push_back(newpointstwo.points[0]);
			j->points.push_back(newpointstwo.points[1]);

			planeexist = true;
			break;
		}

	/*generating a new plane*/				
	if(!planeexist) {
		cplane newc;
		newc.added = checkswitch;
		newc.slopex = newpoints.slope;

		newc.points.clear();
		newc.points.push_back(newpoints.points[0]);
		newc.points.push_back(newpoints.points[1]);
		newc.points.push_back(newpointstwo.points[0]);
		newc.points.push_back(newpointstwo.points[1]);

		/*setting planes color*/
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


void imgd::processpoint(unsigned int point, cplane& setplane) {
	if(twopixarray[point].added == checkswitch)
		return;
	
	bool added = false;
	/*checking up*/
	if(twopixarray[point].points[0].y > 1) {
		if(checktwopoints(point, point - tpwidth) == 1) {
		processpoint(point - tpwidth,setplane);
		added = true;
		}
	}

	/*checking right*/
	if(twopixarray[point].points[1].x < pixwidth - 2) {
		checktwopoints(point, point + 1);
		processpoint(point - tpwidth,setplane);
		added = true;
	}

	/*checking down*/
	if(twopixarray[point].points[1].y < pixheight - 1) {
		checktwopoints(point, point + tpwidth);
		processpoint(point - tpwidth,setplane);
		added = true;
	}

	/*checking left*/
	if(twopixarray[point].points[0].x > 1) {
		checktwopoints(point, point - 1);
		processpoint(point - tpwidth,setplane);
		added = true;
	}
}

inline void throughforeground(unsigned int, unsigned int) {


}

