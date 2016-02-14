#ifndef _NAIBRAIN_MAIN
#define _NAIBRAIN_MAIN
#include <stack>

#include "Devices/webcam.h"
#include "Devices/kinect_man.h"
#include "Devices/chipcomm.h"
#include "Memory/world_map.h"
#include "Memory/path_finding.h"
#include "Infoproc/base_state.h"
#include "Infoproc/test_state.h"
#include "utils/nimg.h"
#include "utils/nexcep.hpp"

/*
	* Handles all the sensors and major AI classes
	*
	* Also should be the only class to make changes to the memory map
*/

class naibrain {
	public:
		naibrain();
		~naibrain();
		void gentest();
		std::vector<nimg*> &GetImages(unsigned int); /*get list of images from specified flags*/
		const world_map *GetMap(); /*Get world map*/
		path_finding &GetPfind(); /*Get path to a point from path find object*/
		void tick(); /*process every frame*/

		//TODO remove
		bool KStatus(); /*returns whether or not kinect is working*/

	private:
		/*devices*/
		kinectman* kinect_manager; /*kinect manager object*/
 		std::vector<nimg*> Images; /*List of images from sensors*/
		nwebc *bcwebcam; /*webcam object*/
		ccomm *drivechip; /*Arduino chip*/

		/*ai objects*/
		world_map wmap; /*world map object*/
		path_finding pfind; /*path find object*/
		std::stack<base_state*> states; /*list of states*/
		
};

#endif
