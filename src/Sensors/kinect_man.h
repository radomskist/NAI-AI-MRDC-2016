#ifndef _NAIBRAIN_EYE_KINECT
#define _NAIBRAIN_EYE_KINECT

#pragma once

#include "Sensors/img_base.h"
#include "Sensors/img_depth.h"
#include "Memory/objects.hpp"
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


/****************************************************************
   FUNCTION:	This will manage all the kinect processes

   NOTES:		N/A

****************************************************************/
enum {NAI_DIS_CAM, NAI_DIS_DEP, NAI_DIS_MIX} nai_dismode; /*Modes in enum form*/

class kinectman {
	public:
		kinectman();
		~kinectman();

		nimg *GetDepthImg();
		nimg *GetRGBImg();

		bool ProcessImages(); //Proccess the images and sets the buffer values

	private:
		void clean();

		/*whether or not freenect is connected*/
		bool nectfailed;
		nimg kdepth;
		nimg krgb;

		/*freenect values*/
 		libfreenect2::Freenect2Device *f2dev;
		libfreenect2::PacketPipeline *f2pipe;
		libfreenect2::SyncMultiFrameListener *nailist;
		libfreenect2::FrameMap nfmap;
		libfreenect2::Freenect2 *f2init;

		/*image processing objects*/
		imgd depth_proc;

};

#endif
