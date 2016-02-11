#ifndef _NAIBRAIN_EYE_IMG_RGB
#define _NAIBRAIN_EYE_IMG_RGB

#pragma once

#include "utils/nimg.h"
#include "utils/ntime.hpp"
#include "Memory/objects.hpp"
#include <vector>
#include <array>
#include <math.h>
#include <cstdlib>
#include <iostream>
#include <opencv2/photo.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

/****************************************************************
   AUTHOR:		Steven Radomski

   FUNCTION:	This will process the kinect RGB image

   NOTES:		N/A

****************************************************************/

class imgrgb {
	public:
		imgrgb();
		~imgrgb();

		void ProcessImg(unsigned char *);
		void findground(cv::Mat&);
		nimg *GetImg();

	private:
		nimg krgb;
		cv::Mat groundmat;
};

#endif
