/*****************************************************************
	* Author: Steven Radomski radomskist@yahoo.com
	*
	* Copyright (C) 2017 Steven Radomski
	* 
	* This file was part of the 2016 NIU robotics AI robot project
	* 
	* This code cannot be copied or used without the permission of
	* the author
	*
	*
*****************************************************************/
#ifndef _NAIBRAIN_EYE_IMG_RGB
#define _NAIBRAIN_EYE_IMG_RGB

#pragma once
#include "utils/nimg.h"
#include "utils/ntime.hpp"
#include "Infoproc/objects.hpp"
#include <vector>
#include <array>
#include <math.h>
#include <cstdlib>
#include <iostream>
#include <opencv2/photo.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <zbar.h>
/****************************************************************
   FUNCTION:	This will process the kinect RGB image

   NOTES:		N/A

****************************************************************/

class imgrgb {
	public:
		imgrgb(unsigned char);
		~imgrgb();

		void ProcessImg(unsigned char *);
		nimg *GetImg();
		void qrscan(int *, std::string *);
		//TODO filter the image for green (QR code), blue (QR code), black (Posts), and our ball color
		void ScanForKeypoints() {};

		//return forward
		bool GroundCheck(bool &, bool&);

		cv::Point2f FindObjColor(unsigned char,unsigned char);


	private:
		int floortol;
		unsigned int floory;
		inline void findground(cv::Mat&);
		inline void findballs(cv::Mat&, cv::Mat&);

		unsigned char ballcolor;
		unsigned char floorcolor;
		nimg krgb;
		cv::Mat objloc;
		cv::Mat groundmat;
		cv::Mat hsvchan[3];
		cv::Ptr<cv::SimpleBlobDetector> balldet;
		cv::Ptr<cv::SimpleBlobDetector> squaredet;

		zbar::ImageScanner zbarscan;
};

#endif
