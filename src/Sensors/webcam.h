#ifndef NAI_WEBCAM
#define NAI_WEBCAM

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/videoio.hpp>
#include "utils/nexcep.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "utils/nimg.h"

class nwebc {
	public:
		nwebc(int);
		~nwebc();
		nimg *GetImg();

	private:
		cv::Mat img;
		nimg camimg;
		cv::VideoCapture *cap;
};


#endif
