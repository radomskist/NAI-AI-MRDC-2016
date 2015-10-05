#include "Point.h"
#include <cmath>


Point::Point()
{
	x = -1;
	y = -1;
	highlighted = false;
}

Point::Point(int initX, int initY)
{
	x = initX;
	y = initY;
	highlighted = false;
}


Point::~Point()
{
}

void Point::setLUV(double l, double u, double v)
{
	this->l = l;
	this->u = u;
	this->v = v;
}

double Point::distanceFrom(Point *p) {
	double l = pow(this->l - p->l, 2);
	double u = pow(this->u - p->u, 2);
	double v = pow(this->v - p->v, 2);

	return sqrt(l + u + v);
}

void Point::colorizeBlack() {
	b = g = r = 0;
}

void Point::colorizeWhite() {
	b = g = r = 255;
}