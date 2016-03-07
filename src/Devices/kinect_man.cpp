#include "Devices/kinect_man.h"

kinectman::kinectman(unsigned char set_color) : rgb_proc(set_color) {

	floory = 300;
	ballcolor = set_color;
	/*********************
	Initializing freenect
	*********************/
	std::cout << "\nNaiEye: Initializing freenect" << std::endl;
	f2init = new libfreenect2::Freenect2;
	f2dev = NULL;
	f2pipe = NULL;
	nailist = NULL;

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
		std::cout << "====================================================================" << std::endl;
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

	bool no;
	ProcessImages();
	depth_proc.ScanGround(no,no);
	std::cout << "\nNaiEye: Video initialized!" << std::endl;
}


float kinectman::qrscan( int * offcenter, std::string *code_text) {
	rgb_proc.qrscan(offcenter,code_text);

	//return kinect distance;
}

bool kinectman::PathCheck(bool &left , bool& right) {

	//If RGB fails do a second scan
	bool returnforward;

	if(!(returnforward = rgb_proc.GroundCheck(left,right)))
		return depth_proc.ScanGround(left,right);

	return returnforward;
}
float kinectman::GetDist(float set_pos) {
	return depth_proc.averagepoints(set_pos);
}

void kinectman::clean() {

	if(nailist != NULL)
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
	return rgb_proc.GetImg();
}

std::vector<obj_plane>& kinectman::GetPlanes() { 
	return planeset;
}

bool kinectman::ProcessImages() {
	if(f2dev == NULL)
		return false;

	nailist->release(nfmap);
	nailist->waitForNewFrame(nfmap);

	//If no frame end
	if(nfmap[libfreenect2::Frame::Depth]->data == NULL)
		return false;

	//Processing depth
	depth_proc.ProcessImg(nfmap[libfreenect2::Frame::Depth]->data,planeset);
	//RGB process
	rgb_proc.ProcessImg(nfmap[libfreenect2::Frame::Color]->data);

	return true;
}
