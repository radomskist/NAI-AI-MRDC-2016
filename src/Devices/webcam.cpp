#include "Devices/webcam.h"

nwebc *naiwebc::createwebcam(std::string ofname) {
	
    int file;
    v4l2_capability cap;
	int iterator = 0;
	std::string directory = "/dev/video";

	while(true) {
		std::string cur_dir = directory + std::to_string(iterator);

		if((file = open(cur_dir.c_str(), O_RDONLY)) == -1){ 
			close(file);
			throw nfail("Camera not found.");
		    return NULL;
		}

	   if(ioctl(file, VIDIOC_QUERYCAP, &cap) == -1) {
			close(file);
			throw nfail("Could not access video capacities!");
		}
		std::string cardname(reinterpret_cast<char*>(&cap.card[0]));
		if(ofname == cardname) {
			close(file);
		return new nwebc(iterator);
		}
	iterator++;
	}
	return NULL;
}


nwebc::nwebc(int index) {
	cap = new cv::VideoCapture(index);

    if(!cap->isOpened()) {
		throw nfail("Webcam index failed!");
		return;
	}
	camimg.width = cap->get(CV_CAP_PROP_FRAME_WIDTH);
	camimg.height = cap->get(CV_CAP_PROP_FRAME_HEIGHT);
	camimg.depth = 3;
		
}

nwebc::~nwebc() {
	cap->release();
	delete cap;
}

nimg *nwebc::GetImg() {
    cap->read(img);
	camimg.data = img.data;
	return &camimg;
}


