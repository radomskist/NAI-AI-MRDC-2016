#ifndef _NAIBRAIN_EYE_IMG_DEPTH
#define _NAIBRAIN_EYE_IMG_DETPH

#pragma once

#include "utils/nimg.h"
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

   FUNCTION:	This will process the kinect depth image

   NOTES:		N/A

****************************************************************/

class imgd {
	public:
		imgd();
		~imgd();

		void ProcessImg(unsigned char *);
		inline std::vector<std::array<cv::Point,4>> CalculatePlanes(std::vector<cv::Vec4i> &, std::vector<std::array<cv::Point,2>>&);
		nimg *GetImg();

	private:
		cv::Ptr<cv::SimpleBlobDetector> blob_detector; //Unused probably, but might be needed
		nimg kdepth;
};

#endif
