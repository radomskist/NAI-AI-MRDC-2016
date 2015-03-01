#include "Point.h"


Point::Point() {
	this->x = -1;
	this->y = -1;
}

Point::Point(int x, int y, int z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Point::~Point() {
}

//void Point::getRandom(int limitX, int limitY) {
//	this->x = rand() % limitX;
//	this->y = rand() % limitY;
//}


Pixels Point::getPixels(GLubyte* data, int width, int height) {
	int startByte = ((this->y * width) + this->x) * 4;
	Pixels pixels;
	pixels.B = data[startByte];
	pixels.G = data[startByte + 1];
	pixels.R = data[startByte + 2];
	pixels.A = data[startByte + 3];
	return pixels;
}
