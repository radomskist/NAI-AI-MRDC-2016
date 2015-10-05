#pragma once

#include <opencv2/core/core.hpp>
#include <stdexcept>
#include <gl/GL.h>
#include <cstdlib>


struct ObjectBucket {
	std::vector<cv::Point3f> points;
	int number;
};

struct ColorPoint {
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
	double L;
	double U;
	double V;
	int x;
	int y;
};


template<int WIDTH, int HEIGHT>
class ObjectSegmentor
{
public:
	ObjectSegmentor(GLubyte* colorData, USHORT* depthData);
	~ObjectSegmentor();
	cv::Point3f* getPoint(int x, int y);
	void findObjects();
	int numberOfBuckets() { return this->buckets.size(); }
	float colorDifference(cv::Point3f *p1, cv::Point3f *p2);
	ObjectBucket* getBucket(int i);

	std::vector<ObjectBucket> buckets;
private:
	std::vector<cv::Point3f> points;
	std::bitset<WIDTH * HEIGHT> checkedStatus;
	std::vector<cv::Point3f> queue;
	std::vector<ColorPoint> colorData;
	const float MAX_DIFFERENCE = 0.0180f; //0.0625f


	cv::Point3f getFirstUncheckedPoint();
	std::vector<cv::Point3f> searchNeighborsForSimilarColor(cv::Point3f *p);
	void getXYZ(ColorPoint *colorData, double(&XYZ)[3]);
	void multiplyMatrices(double(&m1)[3][3], double(&m2)[3][1], double(&result)[3][1]);
	void setLuv(ColorPoint *colorData);
	ColorPoint* getColorData(int x, int y);
	ColorPoint* getColorData(cv::Point3f * p);
};


template <int WIDTH, int HEIGHT>
ObjectSegmentor<WIDTH, HEIGHT>::ObjectSegmentor(GLubyte* colorData, USHORT* depthData) {
	// Reserve space in the vector
	this->points.reserve(WIDTH * HEIGHT);


	// Add all the values to the point cloud
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			int startNdx = ((y * WIDTH) + x) * 4;
			ColorPoint color;
			color.b = colorData[startNdx + 0];
			color.g = colorData[startNdx + 1];
			color.r = colorData[startNdx + 2];
			color.a = colorData[startNdx + 3];
			this->setLuv(&color);
			this->colorData.push_back(color);

			// Add the point to the pointCloud
			this->points.push_back(cv::Point3f(x, y, depthData[(y * WIDTH) + x]));
		}
	}
}

template <int WIDTH, int HEIGHT>
ObjectSegmentor<WIDTH, HEIGHT>::~ObjectSegmentor() {}


template <int WIDTH, int HEIGHT>
cv::Point3f* ObjectSegmentor<WIDTH, HEIGHT>::getPoint(int x, int y) {
	if ((x >= 0 && x < WIDTH) && (y >= 0 && y < HEIGHT)) {
		// Return a the requested point from the pointcloud
		return &this->points.at((WIDTH * y) + x);
	}
	else {
		throw std::out_of_range("Coordinates are invalid");
	}
}

template <int WIDTH, int HEIGHT>
ObjectBucket* ObjectSegmentor<WIDTH, HEIGHT>::getBucket(int i) {
	if (this->buckets.size() > i) {
		return &this->buckets[i];
	}
	else {
		return &ObjectBucket();
	}
}

template <int WIDTH, int HEIGHT>
void ObjectSegmentor<WIDTH, HEIGHT>::findObjects() {
	// Make sure the queue is empty
	queue.clear();

	// Reserve a queue size to reduce having to keep increasing the queue
	queue.reserve(WIDTH * HEIGHT);

	// Add the first point to the queue
	queue.push_back(this->getFirstUncheckedPoint());

	// Clear the bitset
	this->checkedStatus.reset();

	// Create and add a new bucket
	ObjectBucket bucket;
	this->buckets.push_back(bucket);
	std::cout << "NEW LOOP" << std::endl << std::endl;
	int currentBucket = (this->buckets.size() - 1); // Should be zero
	while (!this->checkedStatus.all()) {
		if (this->queue.size() == 0) {
			// Add the first unchecked pixel to the queue
			this->queue.push_back(this->getFirstUncheckedPoint());
			if (this->buckets[currentBucket].points.size() < (WIDTH * HEIGHT * 0.0005)) {
				this->buckets[currentBucket].points.clear();
			}
			else {
				// Create and add a new bucket
				ObjectBucket bucket;
				this->buckets.push_back(bucket);

				// Get the number of the newest bucket
				currentBucket = (this->buckets.size() - 1);
			}
		}
		//std::cout << "Next Item in Queue: " << this->queue.back() << std::endl;
		// Get the next item in the queue and remove it
		cv::Point3f point = this->queue.back();
		this->queue.pop_back();
		
		// Add the point to the bucket
		this->buckets[currentBucket].points.push_back(point);
		this->checkedStatus[(WIDTH * point.y) + point.x] = 1;

		// Search for similar points to add to the queue
		std::vector<cv::Point3f> similarNeighbors = this->searchNeighborsForSimilarColor(&point);

		std::vector<cv::Point3f>::iterator it;
		it = queue.end();
		queue.insert(it, similarNeighbors.begin(), similarNeighbors.end());

	}
	std::cout << "B: " << this->buckets.size() << "\t";
	std::cout << "CB: " << currentBucket << "\t";
	std::cout << "Q: " << this->queue.size() << "\t";
	std::cout << "B: " << this->checkedStatus.count() << std::endl;
}

template <int WIDTH, int HEIGHT>
cv::Point3f ObjectSegmentor<WIDTH, HEIGHT>::getFirstUncheckedPoint() {
	// Loop through all the status of all the pixels
	// and find the first unchecked bit
	for (int i = 0; i < this->checkedStatus.size(); i++) {
		if (this->checkedStatus[i] == 0) {
			int y = i / WIDTH;
			int x = i % WIDTH;
			this->checkedStatus[(WIDTH * y) + x] = 1;

			return *(this->getPoint(x, y));
		}
	}
}

template <int WIDTH, int HEIGHT>
std::vector<cv::Point3f> ObjectSegmentor<WIDTH, HEIGHT>::searchNeighborsForSimilarColor(cv::Point3f *p) {
	std::vector<cv::Point3f> points;

	cv::Point3f *point;

	// Top Point
	try {
		point = this->getPoint(p->x, p->y - 1);
		if (this->checkedStatus[(WIDTH * point->y) + point->x] == 0 && this->colorDifference(p, point) < MAX_DIFFERENCE) {
			this->checkedStatus[(WIDTH * point->y) + point->x] = 1;
			// Add the point to the results
			points.push_back(*point);
		}
	}
	catch (std::out_of_range e) {}

	// Bottom Point
	try {
		point = this->getPoint(p->x, p->y + 1);
		if (this->checkedStatus[(WIDTH * point->y) + point->x] == 0 && this->colorDifference(p, point) < MAX_DIFFERENCE) {
			this->checkedStatus[(WIDTH * point->y) + point->x] = 1;
			// Add the point to the results
			points.push_back(*point);
		}
	}
	catch (std::out_of_range e) {}

	// Left Point
	try {
		point = this->getPoint(p->x - 1, p->y);
		if (this->checkedStatus[(WIDTH * point->y) + point->x] == 0 && this->colorDifference(p, point) < MAX_DIFFERENCE) {
			this->checkedStatus[(WIDTH * point->y) + point->x] = 1;
			// Add the point to the results
			points.push_back(*point);
		}
	}
	catch (std::out_of_range e) {}

	// Right Point
	try {
		point = this->getPoint(p->x + 1, p->y);
		if (this->checkedStatus[(WIDTH * point->y) + point->x] == 0 && this->colorDifference(p, point) < MAX_DIFFERENCE) {
			this->checkedStatus[(WIDTH * point->y) + point->x] = 1;
			// Add the point to the results
			points.push_back(*point);
		}
	}
	catch (std::out_of_range e) {}

	return points;
}

template <int WIDTH, int HEIGHT>
float ObjectSegmentor<WIDTH, HEIGHT>::colorDifference(cv::Point3f *p1, cv::Point3f *p2) {
	ColorPoint *p1_color, *p2_color;

	p1_color = this->getColorData(p1);
	p2_color = this->getColorData(p2);


	return pow(p2_color->L - p1_color->L, 2) +
		pow(p2_color->U - p1_color->U, 2) +
		pow(p2_color->V - p1_color->V, 2);
}

template <int WIDTH, int HEIGHT>
void ObjectSegmentor<WIDTH, HEIGHT>::getXYZ(ColorPoint *colorData, double(&XYZ)[3]) {
	//imageData format is BGRA
	// SET X (aka R)
	// SET Y (aka G)
	// SET Z (aka B)
	XYZ[0] = (double) colorData->r / 255.0;
	XYZ[1] = (double) colorData->g / 255.0;
	XYZ[2] = (double) colorData->b / 255.0;
}

template <int WIDTH, int HEIGHT>
void ObjectSegmentor<WIDTH, HEIGHT>::multiplyMatrices(double(&m1)[3][3], double(&m2)[3][1], double(&result)[3][1]) {
	for (int i = 0; i < 3; i++) {
		double sum = 0;
		for (int k = 0; k < 3; k++) {
			sum += m1[i][k] * m2[k][0];
		}
		result[i][0] = sum;
	}
}

template <int WIDTH, int HEIGHT>
void ObjectSegmentor<WIDTH, HEIGHT>::setLuv(ColorPoint *colorData) {
	double XYZ[3];
	this->getXYZ(colorData, XYZ);

	double colorMatrix[3][1] {
		{ XYZ[0] },
		{ XYZ[1] },
		{ XYZ[2] }
	};


	double matrix[3][3] = {
		{ 0.412453, 0.357580, 0.180423 },
		{ 0.212671, 0.715160, 0.072169 },
		{ 0.019334, 0.119193, 0.950227 }
	};

	double result[3][1] = { {}, {}, {} };

	// Muliply by Matrix
	this->multiplyMatrices(matrix, colorMatrix, result);


	double epsilon = 0.008856;
	double kappa = 903.3;
	double whiteReference[3] = { 95.047, 100.0, 108.883 };
	double Yr = colorMatrix[1][0] / whiteReference[1];
	double u_prime = (4 * XYZ[0]) / (XYZ[0] + 15 * XYZ[1] + 3 * XYZ[2]);
	double v_prime = (9 * XYZ[1]) / (XYZ[0] + 15 * XYZ[1] + 3 * XYZ[2]);
	double u_prime_ref = (4 * whiteReference[0]) / (whiteReference[0] + 15 * whiteReference[1] + 3 * whiteReference[2]);
	double v_prime_ref = (9 * whiteReference[1]) / (whiteReference[0] + 15 * whiteReference[1] + 3 * whiteReference[2]);

	double L;
	if (Yr > epsilon) {
		L = 116 * pow(Yr, 0.5) - 16;
	}
	else {
		L = kappa * Yr;
	}

	// Store Luv to pixeldata
	colorData->L = L;
	colorData->U = 13 * L * (u_prime - u_prime_ref);
	colorData->V = 13 * L * (v_prime - v_prime_ref);
}

template <int WIDTH, int HEIGHT>
ColorPoint* ObjectSegmentor<WIDTH, HEIGHT>::getColorData(int x, int y) {
	return &this->colorData.at((WIDTH * y) + x);
}


template <int WIDTH, int HEIGHT>
ColorPoint* ObjectSegmentor<WIDTH, HEIGHT>::getColorData(cv::Point3f *p) {
	return this->getColorData(p->x, p->y);
}