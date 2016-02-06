#ifndef _NAIBRAIN_EYE_IMG_DEPTH
#define _NAIBRAIN_EYE_IMG_DETPH

#pragma once

#include "utils/nimg.h"
#include <vector>
#include <math.h>
#include <cstdlib>
#include <iostream>
#include <opencv2/photo.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

/****************************************************************
   AUTHOR:		Steven Radomski

   FUNCTION:	This will process the kinect depth image

   NOTES:		N/A

****************************************************************/
struct cpixstr {unsigned char value; unsigned int pos; int x, y;}; /*structure for pixels being checked*/
struct ctwopix {float slope; cpixstr *points[2]; bool added;}; /*structure 2pixels being checked*/

struct cplane {float slopex,slopey; std::vector<cpixstr> points; unsigned char r,g,b, minx,maxx,miny,maxy; bool added;}; /*structure for pixels being checked*/

class imgd {
	public:
		imgd();
		~imgd();

		void ProcessImg(unsigned char *);
		nimg *GetImg();

	private:

		/*image processing*/
		unsigned int pixwidth; /*amount of checking pixels per row/col*/
		unsigned int pixheight;
		unsigned int checktot;/*total number of checking pixels*/
		unsigned int *imcheckpos; /*Array storing location of checking pixels*/

		/*twopix values*/
		int tptot;
		int tpwidth;
		int tpheight;

		cpixstr *cpixlist;
		std::vector<cplane> cplanelist;
		ctwopix* twopixarray;
		bool checkswitch; /*check if value has been added to lits*/
		const int iteration;
		cv::Ptr<cv::SimpleBlobDetector> blob_detector;
		nimg kdepth;
};

#endif
