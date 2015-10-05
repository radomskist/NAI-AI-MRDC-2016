#pragma once
#include "Point.h"
#include <math.h>

class Plane
{
public:
	Plane(Point p1, Point p2, Point p3);
	~Plane();
	double distanceFromPlane(Point* p);
	double getA() { return this->a; };
	double getB() { return this->b; };
	double getC() { return this->c; };
	double getD() { return this->d; };

private:
	void calculatePlaneFromPoints();

	Point p1, p2, p3;
	double a, b, c, d;			// Constants on equation of plane  (aX + bY + cZ + d = 0)
	double denominator;			// Store bottom of distance equation to speed up when calculating the distance between many points
};

