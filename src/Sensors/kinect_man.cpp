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
	#ifdef LIBFREENECT2_WITH_OPENGL_SUPPORT
		f2pipe = new libfreenect2::OpenGLPacketPipeline;
	#else
		f2pipe = new libfreenect2::CpuPacketPipeline;
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

	//Initializing frames with their respective resolutions
	kdepth.width = 512;
	kdepth.height = 424;
	kdepth.depth = 3;
	kdepth.data = new unsigned char[kdepth.width * kdepth.height * kdepth.depth];


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

	//If no frame end
	if(nfmap[libfreenect2::Frame::Depth]->data == NULL)
		return false;

	//Getting pointer to RGB
	krgb.data = nfmap[libfreenect2::Frame::Color]->data;

	//Casting data to a float, which is what it's suppose to be
	float *datahold = (float *)&nfmap[libfreenect2::Frame::Depth]->data[4];

	unsigned char normalized;
	unsigned resolution = kdepth.width * kdepth.height;

	//Converting the float into a proper RGB image (rather than 4 split up parts of one float)
	for(int i = 0; i < resolution; i++) {
		////normalize kinect range to 255
		normalized = datahold[i] * 0.06375f; //(4500.0f - 500.0f)/(255)
		for(int j = 0; j < kdepth.depth; j++) //3 because ignoring alpha channel in the case that it's 4
			kdepth.data[i*kdepth.depth + j] = normalized;
	}
	depth_proc.ProcessImg(kdepth.data);

	return true;
}
