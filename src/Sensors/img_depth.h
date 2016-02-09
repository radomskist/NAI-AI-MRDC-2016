#ifndef _NAIBRAIN_EYE_IMG_DEPTH
#define _NAIBRAIN_EYE_IMG_DETPH

#pragma once

#include "utils/nimg.h"
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

   FUNCTION:	This will process the kinect depth image

   NOTES:		N/A

****************************************************************/

class imgd {
	public:
		imgd();
		~imgd();

		void ProcessImg(unsigned char *);
		nimg *GetImg();

	private:
		//averages area around a point
		inline int averagepoints(cv::Point);

		//Merges all lines together and estimates where unseen lines might be.
		//returns a vector of estimated lines 
		//Arguments, vector of lines as input, vector of horizontal lines to return, vector of verticle lines to return
		inline std::vector<std::array<cv::Point,4>> ProcessLines(std::vector<cv::Vec4i> &, std::vector<std::array<cv::Point,2>>&, std::vector<std::array<cv::Point,2>>&);

		//Calulates planes from lines
		//Returns corners of the plane
		//Horizontal lines as input, Verticle lines as input.
		inline std::vector<obj_plane> CalculatePlanes(std::vector<std::array<cv::Point,2>>&, std::vector<std::array<cv::Point,2>>&, std::vector<std::array<cv::Point,4>> &);

		//TODO Break up into multiple planes
		//Checks if a detected plane is solid and then turns it into an obj
		//Also cuts up a plane into mini planes if it discovers that the plane has a gap
		//Takes in a vector of points to a plane as input, vector of planes as output
		inline void ConvertToObj(std::vector<std::array<cv::Point,4>> &, std::vector<obj_plane>&);

		nimg kdepth;
		int slopeerrorrange;
		int pixdist;
		bool lineest; //Whether or not estimated lines should be calculated
		float *datahold;
		unsigned char *filtered;

};

#endif
