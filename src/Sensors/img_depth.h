#ifndef _NAIBRAIN_EYE_IMG_DEPTH
#define _NAIBRAIN_EYE_IMG_DETPH

#pragma once

#include "utils/nimg.h"
#include "Sensors/img_base.h"
#include <vector>
#include <math.h>
#include <cstdlib>
#include <iostream>

/****************************************************************
   AUTHOR:		Steven Radomski

   FUNCTION:	This will process the kinect depth image

   NOTES:		N/A

****************************************************************/
struct cpixstr {unsigned char value; unsigned int pos; int x, y; bool added;}; /*structure for pixels being checked*/
struct ctwopix {float slope; cpixstr points[3]; bool right;}; /*structure 2pixels being checked*/

struct cplane {float slopex,slopey; std::vector<cpixstr> points; unsigned char r,g,b; bool added;}; /*structure for pixels being checked*/

class imgd : public imgb {
	public:
		imgd();
		~imgd();

		void ProcessImg(unsigned char *);

	private:
		bool recurscan(float &, unsigned char, int, int&, bool, std::vector<cpixstr> &) {};
		void configimg(int,unsigned int, unsigned int, float);

		/*image processing*/
		unsigned int pixwidth; /*amount of checking pixels per row/col*/
		unsigned int pixheight;
		unsigned int checktot;/*total number of checking pixels*/
		unsigned int *imcheckpos; /*Array storing location of checking pixels*/

		cpixstr *cpixlist;
		std::vector<cplane> cplanelist;
		std::vector<ctwopix> twopixarray;
		bool checkswitch; /*check if value has been added to lits*/
		const int iteration;

};

#endif
