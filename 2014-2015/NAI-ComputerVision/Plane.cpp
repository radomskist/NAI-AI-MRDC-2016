#include "Plane.h"


Plane::Plane(Point p1, Point p2, Point p3) {
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;

	this->calculatePlaneFromPoints();
}


Plane::~Plane() {
}

void Plane::calculatePlaneFromPoints() {
	this->a = ((this->p2.getY() - this->p1.getY()) * (this->p3.getZ() - this->p1.getZ())) - ((this->p3.getY() - this->p1.getY()) * (this->p2.getZ() - this->p1.getZ()));
	this->b = ((this->p2.getZ() - this->p1.getZ()) * (this->p3.getX() - this->p1.getX())) - ((this->p3.getZ() - this->p1.getZ()) * (this->p2.getX() - this->p1.getX()));
	this->c = ((this->p2.getX() - this->p1.getX()) * (this->p3.getY() - this->p1.getY())) - ((this->p3.getX() - this->p1.getX()) * (this->p2.getY() - this->p1.getY()));

	this->d = -1 * ((this->p1.getX() * a) + (this->p1.getY() * b) + (this->p1.getZ() * c));

	// Calculate the denominator of the distance from point to plane equation to speed up
	// calculations when calculating distance from point and plane
	this->denominator = sqrt(pow(a,2) + pow(b,2) + pow(c,2));
}

double Plane::distanceFromPlane(Point* p) {
	double numerator = ((this->a * p->getX()) + (this->b * p->getY()) + (this->c * p->getZ()) + this->d);

	return abs(numerator / this->denominator);
}