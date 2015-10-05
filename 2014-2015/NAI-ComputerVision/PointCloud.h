#pragma once

#include <opencv2/core/core.hpp>
#include <stdexcept>

struct ObjectBucket {
	std::vector<cv::Point3f> points;
	int number;
};


template<int WIDTH, int HEIGHT>
class PointCloud
{
public:
	PointCloud(USHORT* values);
	~PointCloud();
	cv::Point3f getPoint(int x, int y);
	void findObjects();
	int numberOfBuckets() { return this->buckets.size(); }

private:
	std::vector<cv::Point3f> points;
	std::vector<ObjectBucket> buckets;
	std::bitset<WIDTH * HEIGHT> checkedStatus;
	std::vector<cv::Point3f> queue;

	cv::Point3f getFirstUncheckedPoint();
	std::vector<cv::Point3f> searchNeighborsForSimilarColor();
};


template <int WIDTH, int HEIGHT>
PointCloud<WIDTH, HEIGHT>::PointCloud(USHORT* values) {
	// Reserve space in the vector
	this->points.reserve(WIDTH * HEIGHT);


	// Add all the values to the point cloud
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {

			// Add the point to the pointCloud
			this->points.push_back(cv::Point3f(w, h, values[(h * WIDTH) + w]));
		}
	}
}

template <int WIDTH, int HEIGHT>
PointCloud<WIDTH, HEIGHT>::~PointCloud() {
	delete this->points;
	delete this->buckets;
	delete this->checkedStatus;
	delete this->queue;
}

template <int WIDTH, int HEIGHT>
cv::Point3f PointCloud<WIDTH, HEIGHT>::getPoint(int x, int y) {
	// Return a the requested point from the pointcloud
	return this->points.at((WIDTH * y) + x);
}

template <int WIDTH, int HEIGHT>
void PointCloud<WIDTH, HEIGHT>::findObjects() {
	// Reserve a queue size to reduce having to keep increasing the queue
	queue.reserve((int)(WIDTH * HEIGHT * 0.25));

	// Make sure the queue is empty
	queue.clear();

	// Add the first point to the queue
	queue.push_back(this->getPoint(0, 0));

	int count = 0; // Temporary loop max
	int currentBucket = 0;
	while (!this->checkedStatus.all() && this->queue.size() > 0 && count < 50) {
		if (this->queue.size() == 0) {
			// Add the first unchecked pixel to the queue
			this->queue.push_back(this->getFirstUncheckedPoint());

			// Create and add a new bucket
			ObjectBucket bucket;
			this->buckets.push_back(bucket);

			// Get the number of the newest bucket
			currentBucket = (this->buckets.size() - 1);
		}

		// Get the next item in the queue and remove it
		cv::Point3f point = this->queue.back();
		this->queue.pop_back();

		// Add the point to the bucket
		this->buckets.at(currentBucket).points.push_back(point);

		// Search for similar points to add to the queue
		

		count++;
	}
}

template <int WIDTH, int HEIGHT>
cv::Point3f PointCloud<WIDTH, HEIGHT>::getFirstUncheckedPoint() {
	// Loop through all the status of all the pixels
	// and find the first unchecked bit
	for (int i = 0; i < this->checkedStatus.size(); i++) {
		if (this->checkedStatus[i] == 0) {
			int y = i / WIDTH;
			int x = i % WIDTH;
			return this->queue.push_back(this->getPoint(x, y));
		}
	}
}