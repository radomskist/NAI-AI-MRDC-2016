#ifndef _NAIBRAIN_MAIN
#define _NAIBRAIN_MAIN
#include "Sensors/kinect_man.h"
#include "Memory/world_map.h"
enum SensorFlags {
	KRGB = 0x01,	//Kinect rgb
	KDEP = 0x02,	//Kinect depth

};

class naibrain {
	public:
		naibrain();
		~naibrain();
		std::vector<unsigned char *> &GetImages(unsigned int);

		bool KStatus(); /*returns whether or not kinect is working*/
	private:
		kinectman* kinect_manager;
		std::vector<unsigned char *> Images;
		world_map worldmap;
};

#endif
