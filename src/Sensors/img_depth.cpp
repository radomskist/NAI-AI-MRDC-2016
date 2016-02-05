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
		int ret_int = ((cpixlist[i].y*gapx*2 + (i+gapx))*pixjumpx + (cpixlist[i].y + gapy) * pixjumpy);

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

	for(int i = 0; i < tptot; i++) {
		twopixarray[i].points[0] = &cpixlist[i*2];
		twopixarray[i].points[1] = &cpixlist[i*2 + 1];
		twopixarray[i].added = !checkswitch;
	}

	if(tpwidth % 2 != 0)
		tpwidth--;
}

/*checking two points*/
int imgd::checktwopoints(unsigned int first, unsigned int second) {
	float testslope = twopixarray[second].slope - twopixarray[first].slope;
	std::cout << twopixarray[second].points[0]->x << ":  " << twopixarray[second].slope << " - " << twopixarray[first].slope << " = " << testslope << std::endl;

	//TODO: Noise check
	if(fabs(testslope) <= 1) { //Tolerance
		
		
		return 1;
	}


	//Seeing if foreground or background
	if(twopixarray[second].slope > twopixarray[first].slope)
		return 2;
	
	return 0;
}

/*Two pixel slope check suggested by Joel*/
void imgd::ProcessImg(unsigned char *depthbuff) {
	cplanelist.clear();

/*	OpenCv smoothing didn't work. Might try later
	cv::Mat img(512, 424, CV_8UC4, depthbuff);
	cv::Mat newimg;
	cv::Mat outimg;
	img.convertTo(newimg, CV_8UC3);

	try {
		cv::bilateralFilter(newimg, outimg, 9, 150,150);
		}
	catch(std::exception &e) {
		std::cout << e.what()  << std::endl;
	}
	int resolution = 512*424;

	for(int i = 0; i < resolution; i++) {
		depthbuff[i*4] = outimg.data[i*4];
		depthbuff[i*4+1] = outimg.data[i*4];
		depthbuff[i*4+2] = outimg.data[i*4];
		depthbuff[i*4+3] = 255;
	}*/

	/*loading points*/
	for(int i = 0; i < checktot; i++) 
		cpixlist[i].value = depthbuff[imcheckpos[i]];

	/*calculating slopes*/
	for(int i = 0; i < tpwidth; i++)
		twopixarray[i].slope = twopixarray[i].points[0]->value - (twopixarray[i].points[1]->value - twopixarray[i].points[0]->value)*twopixarray[i].points[0]->x;



	/*calling the process function on each point*/
//	for(int i = 0; i < tptot; i++) {
		cplane newc;
		processpoint(0,newc,false);

		if(newc.points.size() > 5) //What's considered a plane(more than this many pixels)
			cplanelist.push_back(newc);

	//}

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
			j->points.push_back(*newpoints.points[0]);
			j->points.push_back(*newpoints.points[1]);
			j->points.push_back(*newpointstwo.points[0]);
			j->points.push_back(*newpointstwo.points[1]);

			planeexist = true;
			break;
		}

	/*generating a new plane*/				
	if(!planeexist) {
		cplane newc;
		newc.added = checkswitch;
		newc.slopex = newpoints.slope;

		newc.points.clear();
		newc.points.push_back(*newpoints.points[0]);
		newc.points.push_back(*newpoints.points[1]);
		newc.points.push_back(*newpointstwo.points[0]);
		newc.points.push_back(*newpointstwo.points[1]);

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


void imgd::processpoint(unsigned int point, cplane& setplane, bool set_added) {
	if(twopixarray[point].added == checkswitch)
		return;
	
	checkswitch = checkswitch;
	bool added = false;

	/*checking up
	if(twopixarray[point].points[0].y > 1) {
		int flag = checktwopoints(point, point - tpwidth);
		if(flag == 1) {
			processpoint(point - tpwidth,setplane, true);
			added = true;
		}
	}*/

	/*checking right*/
	if((twopixarray[point].points[1]->x)/2 < tpwidth - 2) {
		int flag = checktwopoints(point, point + 1);
		if(flag == 1) {
			processpoint(point + 1 ,setplane, true);
			added = true;
		}
	}

	/*checking down
	if(twopixarray[point].points[0].y < pixheight - 2) {
		int flag = checktwopoints(point, point + tpwidth);
		if(flag == 1) {
			processpoint(point + tpwidth,setplane,true);
			added = true;
		}
	}*/

	/*checking left
	if(twopixarray[point].points[0].x > 1) {
		int flag = checktwopoints(point, point - 1);
		if(flag == 1) {
			processpoint(point - 1,setplane, true);
			added = true;
		}
	}*/

	/*seeing if being added to plane*/
	if(!added)
		added = set_added;

	/*if still not added*/
	if(!added)
		checkswitch = !checkswitch;
	else {
		setplane.points.push_back(*twopixarray[point].points[0]);
		setplane.points.push_back(*twopixarray[point].points[1]);
	}
}

inline void throughforeground(unsigned int, unsigned int) {


}

