#ifndef _NAIBRAIN_MAIN
#define _NAIBRAIN_MAIN
#include "Devices/webcam.h"
#include "Devices/kinect_man.h"
#include "Devices/chipcomm.h"
#include "Memory/world_map.h"
#include "Memory/path_finding.h"
#include "utils/nimg.h"
#include "utils/nexcep.hpp"

/*
	* Handles all the sensors and major AI classes
*/

class naibrain {
	public:
		naibrain();
		~naibrain();
		std::vector<nimg*> &GetImages(unsigned int); /*get list of images from specified flags*/
		world_map &GetMap(); /*Get world map*/
		path_finding &GetPfind(); /*Get path to a point from path find object*/
		bool KStatus(); /*returns whether or not kinect is working*/

	private:
		path_finding pfind; /*path find object*/
		kinectman* kinect_manager; /*kinect manager object*/
 		std::vector<nimg*> Images; /*List of images from sensors*/
		world_map wmap; /*world map object*/
		nwebc *bcwebcam; /*webcam object*/
		ccomm *drivechip; /*Arduino chip*/
		
};

#endif
