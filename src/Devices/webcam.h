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
#ifndef NAI_WEBCAM
#define NAI_WEBCAM

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/videoio.hpp>
#include "utils/nexcep.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "utils/nimg.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <unistd.h>

/*
	* This class is primarily intended to manage any extra
	* web-cameras the project might have
*/
class nwebc {
	public:
		nwebc(int);
		~nwebc();
		virtual nimg *GetImg();

	protected:
		cv::Mat img;
		nimg camimg;
		cv::VideoCapture *cap;
};

class bcam :public nwebc {
	public:
		bcam(int);
		~bcam();
		nimg *GetImg();
		void SetColor(unsigned char, unsigned char);
	private:
		unsigned char color, sat;

};

/*This is a factory method for creating a new webcam*/
namespace naiwebc { 
nwebc *createwebcam(std::string,int); //simple factory function for creating webcams
//void listwebcams(); maybe in the future
}


#endif
