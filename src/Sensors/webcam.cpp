#include "Sensors/webcam.h"

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
