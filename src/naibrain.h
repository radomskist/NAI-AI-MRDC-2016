#ifndef _NAIBRAIN_MAIN
#define _NAIBRAIN_MAIN
#include "Sensors/webcam.h"
#include "Sensors/kinect_man.h"
#include "Memory/world_map.h"
#include "Memory/path_finding.h"

enum SensorFlags {
	KRGB = 0x01,	//Kinect rgb
	KDEP = 0x02,	//Kinect depth

};

class naibrain {
	public:
		naibrain();
		~naibrain();
		std::vector<unsigned char *> &GetImages(unsigned int);
		world_map &GetMap();
		std::vector<obj_point> GetPath(obj_point);
		bool KStatus(); /*returns whether or not kinect is working*/
	private:
		path_finding pfind;
		kinectman* kinect_manager;
		std::vector<unsigned char *> Images;
		world_map wmap;
		nwebc bcwebcam;
};

#endif
