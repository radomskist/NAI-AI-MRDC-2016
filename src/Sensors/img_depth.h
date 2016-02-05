#ifndef _NAIBRAIN_EYE_IMG_DEPTH
#define _NAIBRAIN_EYE_IMG_DETPH

#pragma once

#include "utils/nimg.h"
#include "Sensors/img_base.h"
#include <vector>
#include <math.h>
#include <cstdlib>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>

/****************************************************************
   AUTHOR:		Steven Radomski

   FUNCTION:	This will process the kinect depth image

   NOTES:		N/A

****************************************************************/
struct cpixstr {unsigned char value; unsigned int pos; int x, y;}; /*structure for pixels being checked*/
struct ctwopix {float slope; cpixstr *points[2]; bool added;}; /*structure 2pixels being checked*/

struct cplane {float slopex,slopey; std::vector<cpixstr> points; unsigned char r,g,b, minx,maxx,miny,maxy; bool added;}; /*structure for pixels being checked*/

class imgd : public imgb {
	public:
		imgd();
		~imgd();

		void ProcessImg(unsigned char *);

	private:
		void addtoplane(ctwopix&, ctwopix&); /*add two twopix to a plane*/
		inline int checktwopoints(unsigned int, unsigned int); /*check two twopicks*/
		inline void throughforeground(unsigned int, unsigned int); /*See if the plane continues behind foreground*/
	
		void processpoint(unsigned int,cplane&, bool); /*do process function on a point*/

		void configimg(int,unsigned int, unsigned int, float); /*configure scanning space*/

		int recurscan(float &, unsigned char, int, int&, bool, std::vector<cpixstr> &) {};


		/*image processing*/
		unsigned int pixwidth; /*amount of checking pixels per row/col*/
		unsigned int pixheight;
		unsigned int checktot;/*total number of checking pixels*/
		unsigned int *imcheckpos; /*Array storing location of checking pixels*/

		/*twopix values*/
		int tptot;
		int tpwidth;

		cpixstr *cpixlist;
		std::vector<cplane> cplanelist;
		ctwopix* twopixarray;
		bool checkswitch; /*check if value has been added to lits*/
		const int iteration;

};

#endif
