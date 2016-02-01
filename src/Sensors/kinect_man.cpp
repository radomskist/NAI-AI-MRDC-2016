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
	f2pipe = new libfreenect2::CpuPacketPipeline;

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

	kdepth.width = 512;
	kdepth.height = 424;
	kdepth.depth = 4;

	krgb.width = 1920;
	krgb.height = 1080;
	krgb.depth = 4;
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

nimg *kinectman::GetDepthImg() {
	return &kdepth;
}

nimg *kinectman::GetRGBImg() {
	return &krgb;
}

bool kinectman::ProcessImages() {
	nailist->release(nfmap);
	nailist->waitForNewFrame(nfmap);

	if(nfmap[libfreenect2::Frame::Depth]->data == NULL)
		return false;

	krgb.data = nfmap[libfreenect2::Frame::Color]->data;
	float *datahold = (float *)&nfmap[libfreenect2::Frame::Depth]->data;

	if(kdepth.data)
		delete kdepth.data;

	kdepth.data = new unsigned char[512*424*4];
	unsigned char normalized;

	for(int i = 0; i < 217088;i++) {
		normalized = datahold[i] /((4500.0 - 500.0)/(255));
		kdepth.data[i*4] = normalized;
		kdepth.data[i*4 + 1] = normalized;
		kdepth.data[i*4 + 2] = normalized;
		}
	//depth_proc.ProcessImg(nfmap[libfreenect2::Frame::Depth]->data);

	return true;
}
