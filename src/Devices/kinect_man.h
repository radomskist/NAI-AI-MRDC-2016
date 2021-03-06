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
#ifndef _NAIBRAIN_EYE_KINECT
#define _NAIBRAIN_EYE_KINECT

#pragma once

#include "Infoproc/kinect/img_depth.h"
#include "Infoproc/kinect/img_rgb.h"
#include "Infoproc/objects.hpp"
#include "utils/nexcep.hpp"
#include "utils/nimg.h"

/*Standard libraries*/
#include <vector>
#include <iterator>
#include <iostream>
#include <cstdlib>
#include <exception>
#include <functional>

/*needed to use the kinect!*/
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/packet_pipeline.h>
#include <libfreenect2/logger.h>


/****************************************************************
   FUNCTION:	This will manage all the kinect processes

   NOTES:		N/A

	TODO: Make a basic sensor so code can operate without kinect

****************************************************************/
enum {NAI_DIS_CAM, NAI_DIS_DEP, NAI_DIS_MIX} nai_dismode; /*Modes in enum form*/

class kinectman {
	public:
		kinectman(unsigned char);
		~kinectman();

		int straighten();
		bool ProcessImages(); //Proccess the images and sets the buffer values
		bool PathCheck(bool &, bool&);
		std::vector<obj_plane>& GetPlanes();

		float findobj(int *, unsigned char, unsigned char);
		float qrscan(int*, std::string *);

		float GetDist(float);
		nimg *GetDepthImg();
		nimg *GetRGBImg();


	private:
		unsigned char ballcolor;
		unsigned int floory; //y location of floor on images;

		void clean();
		
		/*whether or not freenect is connected*/
		bool nectfailed;
		std::vector<obj_plane> planeset;

		/*freenect values*/
 		libfreenect2::Freenect2Device *f2dev;
		libfreenect2::PacketPipeline *f2pipe;
		libfreenect2::SyncMultiFrameListener *nailist;
		libfreenect2::FrameMap nfmap;
		libfreenect2::Freenect2 *f2init;

		/*image processing objects*/
		imgd depth_proc;
		imgrgb rgb_proc;


};

#endif
