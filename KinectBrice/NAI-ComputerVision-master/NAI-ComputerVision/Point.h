#pragma once
#include "glut.h"
#include <cstdlib>

struct Pixels {
	byte B;
	byte G;
	byte R;
	byte A;
};

class Point
{
public:
	Point();
	Point(int x, int y, int z);
	~Point();
//	void getRandom(int limitX, int limitY, GLubyte* data);
	int getX() { return this->x; };
	int getY() { return this->y; };
	int getZ() { return this->z; };
	Pixels getPixels(GLubyte* data, int width, int height);

private:
	int x, y, z;
};

