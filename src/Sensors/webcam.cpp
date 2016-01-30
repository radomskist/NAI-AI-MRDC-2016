#include "Sensors/webcam.h"

nwebc::nwebc() {
    cv::VideoCapture cap (-1);

    if (!cap.isOpened())
         std::cout << "Cam initialize failed" << std::endl;
    else std::cout << "Cam initialized" << std::endl;

}

nwebc::~nwebc() {

}
