#ifndef _NAIBRAIN_MAIN
#define _NAIBRAIN_MAIN
#include "Sensors/kinect_man.h"

enum SensorFlags {
	KRGB = 0x01,	//Kinect rgb
	KDEP = 0x02,	//Kinect depth

};

class naibrain {
	public:
		naibrain();
		~naibrain();
		std::vector<unsigned char *> &GetImages(unsigned int);

	private:
		kinectman* kinect_manager;
		std::vector<unsigned char *> Images;
};

#endif
