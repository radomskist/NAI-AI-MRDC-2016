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
struct cpixstr {float value; unsigned int pos; int x, y;bool added;}; /*structure for pixels being checked*/
struct cplane {float originx, originy; float slopex,slopey; std::vector<cpixstr> points; unsigned char r,g,b;}; /*structure for pixels being checked*/

class imgd : public imgb {
	public:
		imgd();
		~imgd();

		void ProcessImg(unsigned char *);

	private:
		bool recurscan(unsigned char *, float *, int, int*, bool);
		void configimg(int,unsigned int, unsigned int, float);

		/*image processing*/
		unsigned int pixwidth; /*amount of checking pixels per row/col*/
		unsigned int pixheight;
		unsigned int checktot;/*total number of checking pixels*/
		unsigned int *imcheckpos; /*Array storing location of checking pixels*/

		cpixstr *cpixlist;
		std::vector<cplane> cplanelist;
		bool checkswitch; /*check if value has been added to lits*/

};

#endif
