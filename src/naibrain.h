#ifndef _NAIBRAIN_MAIN
#define _NAIBRAIN_MAIN
#include "Sensors/webcam.h"
#include "Sensors/kinect_man.h"
#include "Memory/world_map.h"
#include "Memory/path_finding.h"
#include "utils/nimg.h"

enum SensorFlags {
	KRGB = 0x01,	//Kinect rgb
	KDEP = 0x02,	//Kinect depth
	BCCAM = 0x04	//Ball check camera

};

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
};

#endif
