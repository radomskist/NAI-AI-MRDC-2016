#pragma once
class Point
{
public:
	Point();
	Point(int x, int y);
	~Point();
	void setLUV(double l, double u, double v);
	void colorizeBlack();
	void colorizeWhite();
	double distanceFrom(Point *p);
	int x;
	int y;
	bool highlighted = false;
	double l, u, v;
	int r, g, b;
};

