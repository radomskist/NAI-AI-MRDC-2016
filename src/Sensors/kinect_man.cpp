#include "Sensors/kinect_man.h"


kinectman::kinectman() {
	/*********************
	Initializing freenect
	*********************/
	std::cout << "\nNaiEye: Initializing freenect" << std::endl;
	f2init = new libfreenect2::Freenect2;
	f2dev = NULL;

	//Checking num of devices
	if(f2init->enumerateDevices() == 0) {
		clean();
		throw nfail("No devices found!");
		return;
	}

	//Initializing the freenect2 pipeline
	f2pipe = new libfreenect2::CpuPacketPipeline();

	if(f2pipe == NULL) {
		clean();
		throw nfail("Opening pipeline failed!");
		return;
	}

	//Opening ocnnection to device
	f2dev = f2init->openDevice(f2init->getDefaultDeviceSerialNumber(), f2pipe);
	if(f2dev == NULL) {
		clean();
		throw nfail("Opening device failed!");
		return;
	}


	std::cout << "\nNaiEye: Freenect initializing succeeded!\nNaiEye: Initializing video" << std::endl;

	nailist = new libfreenect2::SyncMultiFrameListener(libfreenect2::Frame::Color | libfreenect2::Frame::Depth);
	f2dev->setColorFrameListener(nailist);
	f2dev->setIrAndDepthFrameListener(nailist);
	f2dev->start();

	std::cout << "\nNaiEye: Video initialized!" << std::endl;
}

void kinectman::clean() {
	nailist->release(nfmap);

	if(f2dev != NULL) {
		f2dev->stop();
		f2dev->close();
	}
}

kinectman::~kinectman() {
	clean();

}

unsigned char *kinectman::GetDepthImg() {
	return nfmap[libfreenect2::Frame::Depth]->data;
}

unsigned char *kinectman::GetRGBImg() {
	return nfmap[libfreenect2::Frame::Color]->data;
}

bool kinectman::ProcessImages() {
	nailist->release(nfmap);
	nailist->waitForNewFrame(nfmap); // wait .16 seconds for a frame

	if(nfmap[libfreenect2::Frame::Depth]->data == NULL)
		return false;

	for(int i = 0; i < 512*424; i++) {
		unsigned char bob = (nfmap[libfreenect2::Frame::Depth]->data[i*4+2] + nfmap[libfreenect2::Frame::Depth]->data[i*4+3])/2;
		nfmap[libfreenect2::Frame::Depth]->data[i*4] = bob;
		nfmap[libfreenect2::Frame::Depth]->data[i*4+1] = bob;
		nfmap[libfreenect2::Frame::Depth]->data[i*4+2] = bob;
		nfmap[libfreenect2::Frame::Depth]->data[i*4+3] = bob;
	}
		std::cout << "DISTANCE: " << (int)nfmap[libfreenect2::Frame::Depth]->data[(512 * 212 + 256)*4] << std::endl;

	return true;
}
