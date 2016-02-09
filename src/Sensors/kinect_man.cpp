#include "Sensors/kinect_man.h"

kinectman::kinectman() {
	/*********************
	Initializing freenect
	*********************/
	std::cout << "\nNaiEye: Initializing freenect" << std::endl;
	f2init = new libfreenect2::Freenect2;
	f2dev = NULL;
	f2pipe = NULL;

	//Checking num of devices
	if(f2init->enumerateDevices() == 0) {
		clean();
		throw nfail("No devices found!");
		return;
	}

	//Initializing the freenect2 pipeline
	#ifdef LIBFREENECT2_WITH_OPENGL_SUPPORT
		f2pipe = new libfreenect2::OpenGLPacketPipeline;
	#else
		f2pipe = new libfreenect2::CpuPacketPipeline;
		std::cout << "====================================================================" << std::endl;
		std::cout << "ERROR! OPENGL DID NOT COMPILE WITH THE FREENECT2 LIBRARY BEING USED!" << std::endl;
	#endif

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

	//Initializing RGB, depth needs to be processed and can't be given straight
	//so it's in the depth image processor
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
	return depth_proc.GetImg();
}

nimg *kinectman::GetRGBImg() {
	return &krgb;
}

bool kinectman::ProcessImages() {
	if(f2dev == NULL)
		return false;

	nailist->release(nfmap);
	nailist->waitForNewFrame(nfmap);

	//If no frame end
	if(nfmap[libfreenect2::Frame::Depth]->data == NULL)
		return false;

	//Getting pointer to RGB
	krgb.data = nfmap[libfreenect2::Frame::Color]->data;

	//Processing depth
	depth_proc.ProcessImg(nfmap[libfreenect2::Frame::Depth]->data);

	return true;
}
