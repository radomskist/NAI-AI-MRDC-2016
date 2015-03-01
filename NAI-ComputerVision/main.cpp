#include "main.h"


// OpenGL Variables
GLuint textureId;                           // ID of the texture to contain Kinect RGB Data
GLubyte colorData[WIDTH * HEIGHT * 4];      // BGRA array containing the texture data
USHORT depthData[WIDTH * HEIGHT];           // depth data
USHORT mapData[WIDTH * HEIGHT];

bool results = true;

std::bitset<POINTS_IN_CLOUD> checkedStatus;	// Detemine if the current pixel has been checked

Kinect* kinect;

std::vector<cv::Point3f> pointCloud;
std::vector<int> indices(MAX_RESULTS);
std::vector<float> dists(MAX_RESULTS);
std::vector<cv::Point3f> queue;

int cycles = 0;

bool initKinect() {
	kinect = new Kinect();
	return kinect->isInitialized();
}

void drawKinectData() {
	// Get Data From the Kinect
	kinect->getFrameData(Kinect::IMAGE_COLOR, colorData);
	kinect->getDepthFrameData(depthData);
	if (kinect->mapColorFrameToDepthFrame(depthData, mapData)) objectSegmentation();

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH, HEIGHT, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (GLvoid*) colorData);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(WIDTH, 0, 0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(WIDTH, HEIGHT, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, HEIGHT, 0.0f);
	glEnd();
}

int main(int argc, char* argv[]) {
	if (!init(argc, argv)) return 1;
	pointCloud.reserve(WIDTH * HEIGHT);
	for (int x = 0; x < HEIGHT; x++) {
		for (int y = 0; y < WIDTH; y++) {
			pointCloud.push_back(cv::Point3f());
		}
	}
	if (!initKinect()) return 1;


	// Initialize textures
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (GLvoid*) colorData);
	glBindTexture(GL_TEXTURE_2D, 0);

	// OpenGL setup
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);
	glEnable(GL_TEXTURE_2D);

	// Camera setup
	glViewport(0, 0, WIDTH, HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WIDTH, HEIGHT, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Main loop
	execute();
	return 0;
}

void populatePointCloud() {
	// Start Clock to measure time method takes
	auto start = std::chrono::high_resolution_clock::now();

	for (int x = 0; x < HEIGHT; x++) {
		for (int y = 0; y < WIDTH; y++) {
			//int d = getDistanceOfPixel(x, y);

			// Add the point to the pointCloud
			pointCloud[(WIDTH * x) + y] = cv::Point3f(y, x, depthData[(x * WIDTH) + y]);
			//pointCloud.push_back(cv::Point3f(x, y, d));
		}
	}

	// Get the total time in ms the method took
	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	long long microseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

	// Print out the elpsed time of the method
	std::cout << "Generate Point Cloud: " << microseconds << " ms\n";
}


//GLubyte data[WIDTH * HEIGHT * 4];
void objectSegmentation() {
	if (cycles++ < 10) return;
	// Start Clock to measure time method takes
	auto start = std::chrono::high_resolution_clock::now();

	// Populate the point cloud
	populatePointCloud();

	// Set all the points as unsearched
	checkedStatus.reset();
	queue.clear();

	// Add the starting point to the queue
	queue.push_back(cv::Point3f(330, 220, getDistanceOfPixel(330, 220)));

	// Set Search Point
	cv::Point3f searchPoint = queue.back();


	std::cout << "Search Point: " << queue.back() << std::endl;

	// Turn the search Pixel Yellow
	colorData[(((int) searchPoint.y * WIDTH) + (int) searchPoint.x) * 4] = 0;       // B
	colorData[(((int) searchPoint.y * WIDTH) + (int) searchPoint.x) * 4 + 1] = 255; // G
	colorData[(((int) searchPoint.y * WIDTH) + (int) searchPoint.x) * 4 + 2] = 255; // R

	// Make yellow spot larger
	colorData[(((int) searchPoint.y * WIDTH) + (int) (searchPoint.x + 1)) * 4] = 0;     // B
	colorData[(((int) searchPoint.y * WIDTH) + (int) (searchPoint.x + 1)) * 4 + 1] = 255;   // G
	colorData[(((int) searchPoint.y * WIDTH) + (int) (searchPoint.x + 1)) * 4 + 2] = 255;   // R

	colorData[(((int) searchPoint.y * WIDTH) + (int) (searchPoint.x - 1)) * 4] = 0;     // B
	colorData[(((int) searchPoint.y * WIDTH) + (int) (searchPoint.x - 1)) * 4 + 1] = 255;   // G
	colorData[(((int) searchPoint.y * WIDTH) + (int) (searchPoint.x - 1)) * 4 + 2] = 255;   // R

	colorData[(((int) (searchPoint.y + 1) * WIDTH) + (int) searchPoint.x) * 4] = 0;     // B
	colorData[(((int) (searchPoint.y + 1) * WIDTH) + (int) searchPoint.x) * 4 + 1] = 255;   // G
	colorData[(((int) (searchPoint.y + 1) * WIDTH) + (int) searchPoint.x) * 4 + 2] = 255;   // R

	colorData[(((int) (searchPoint.y - 1) * WIDTH) + (int) searchPoint.x) * 4] = 0;     // B
	colorData[(((int) (searchPoint.y - 1) * WIDTH) + (int) searchPoint.x) * 4 + 1] = 255;   // G
	colorData[(((int) (searchPoint.y - 1) * WIDTH) + (int) searchPoint.x) * 4 + 2] = 255;   // R


	// Start Clock to measure time search takes
	auto start_create_tree = std::chrono::high_resolution_clock::now();


	// KdTree with 4 random trees
	cv::flann::KDTreeIndexParams indexParams(NUM_SEARCH_TREES);
	cv::flann::Index kdtree(cv::Mat(pointCloud).reshape(1), indexParams);


	// Get the total time in ms the search took
	auto elapsed_create_tree = std::chrono::high_resolution_clock::now() - start_create_tree;
	long long create_tree_microseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_create_tree).count();

	// Print out the elpsed time of the method
	std::cout << "Creating Tree: " << create_tree_microseconds << " ms\n";

	std::vector<float> query;
	//query.reserve(3);
	query.push_back(searchPoint.x);
	query.push_back(searchPoint.y);
	query.push_back(searchPoint.z);


	std::cout << "Searching for Nearest Neighbors" << std::endl;
	queue.reserve(POINTS_IN_CLOUD);

	int count = 0;
	while (!checkedStatus.all() && queue.size() > 0 && count < MAX_LOOPS) {
		//if (queue.size() % 1000 == 0) std::cout << "Queue Size: " << queue.size() << std::endl;
		//std::cout << "Loop #" << count << std::endl;

		searchPoint = queue.back();		// Get the new search point
		queue.pop_back();				// Remove searchPoint from the queue

		query.clear();			// Clear the query vector
		indices.clear();		// Clear the indicies
		dists.clear();			// Clear the distances

		// Add the searchPoint to the query
		query.push_back(searchPoint.x);
		query.push_back(searchPoint.y);
		query.push_back(searchPoint.z);

		//std::cout << "Search Point: " << searchPoint << std::endl;

		kdtree.radiusSearch(query, indices, dists, SEARCH_RADIUS, MAX_RESULTS, cv::flann::SearchParams(NUM_SEARCHES));


		//int j = 0;
		cv::Point3f point;
		for (int i = 0; i < indices.size(); i++) {
			if (dists[i] > 0 && checkedStatus[indices[i]] == 0) {
				//std::cout << indices[i] << "\t";
				point = pointCloud.at(indices[i]);


				// Turn the pixel black
				colorData[(((int) point.y * WIDTH) + (int) point.x) * 4] = 0;     // B
				colorData[(((int) point.y * WIDTH) + (int) point.x) * 4 + 1] = 0; // G
				colorData[(((int) point.y * WIDTH) + (int) point.x) * 4 + 2] = 0; // R
				queue.push_back(point);
				checkedStatus[indices[i]] = 1;
				//j++;
			}
		}
		//std::cout << "Size: " << j << std::endl;

		//std::cout << "Queue Size: " << queue.size() << std::endl;

		count++;
	}


	// Release the Kd-Tree
	kdtree.release();


	// Get the total time in ms the method took
	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	long long microseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

	// Print out the elpsed time of the method
	std::cout << "Elapsed Time: " << microseconds << " ms\n";
}


//GLubyte data[WIDTH * HEIGHT * 4];
void objectSegmentationOld() {
	// Start Clock to measure time method takes
	auto start = std::chrono::high_resolution_clock::now();


	int x, y;
	// select 3 random points
	Point p1, p2, p3;

	//x = rand() % WIDTH;
	//y = rand() % HEIGHT;
	//x = 80-250;
	//y = 50-290;
	x = 315;
	y = 225;
	p1 = Point(x, y, getDistanceOfPixel(x, y));
	std::cout << getDistanceOfPixel(x, y) << "\t";

	//x = rand() % WIDTH;
	//y = rand() % HEIGHT;
	x = 325;
	y = 220;
	p2 = Point(x, y, getDistanceOfPixel(x, y));
	std::cout << getDistanceOfPixel(x, y) << "\t";

	//x = rand() % WIDTH;
	//y = rand() % HEIGHT;
	x = 320;
	y = 230;
	p3 = Point(x, y, getDistanceOfPixel(x, y));
	std::cout << getDistanceOfPixel(x, y) << "\t";

	Plane plane = Plane(p1, p2, p3);

	double sum = 0;
	double tot = 0;

	for (x = 0; x < WIDTH; x++) {
		for (y = 0; y < HEIGHT; y++) {
			double d = plane.distanceFromPlane(&Point(x, y, getDistanceOfPixel(x, y)));
			tot++;
			sum += d;
			if (d < DISTANCE_THRESHOLD) { // Set to Black
				int offset = ((y * WIDTH) + x) * 4;
				colorData[offset] = 0;      // B
				colorData[offset + 1] = 0;  // G
				colorData[offset + 2] = 0;  // R
			}
			/*else {    // Set to White
			colorData[offset] = 255;
			colorData[offset + 1] = 255;
			colorData[offset + 2] = 255;
			}*/
		}
	}
	//std::cout << sum / tot << "\t";

	x = p1.getX();
	y = p1.getY();
	colorData[((y * WIDTH) + x) * 4] = 0;       // B
	colorData[((y * WIDTH) + x) * 4 + 1] = 255; // G
	colorData[((y * WIDTH) + x) * 4 + 2] = 255; // R

	x = p2.getX();
	y = p2.getY();
	colorData[((y * WIDTH) + x) * 4] = 0;       // B
	colorData[((y * WIDTH) + x) * 4 + 1] = 255; // G
	colorData[((y * WIDTH) + x) * 4 + 2] = 255; // R

	x = p3.getX();
	y = p3.getY();
	colorData[((y * WIDTH) + x) * 4] = 0;       // B
	colorData[((y * WIDTH) + x) * 4 + 1] = 255; // G
	colorData[((y * WIDTH) + x) * 4 + 2] = 255; // R


	// Get the total time in ms the method took
	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	long long microseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

	// Print out the elpsed time of the method
	std::cout << "Elapsed Time: " << microseconds << " ms\n";
}

inline int getDistanceOfPixel(int x, int y) {
	//int offset = (y * WIDTH) + x;

	return depthData[(y * WIDTH) + x];
}

/*

http://code.ros.org/lurker/message/20110426.130608.5943da44.gl.html
filterPointCloudByNearestNeighborDistance


http://stackoverflow.com/questions/17142737/why-opencv-kdtree-always-returns-same-nearest-neighbour-in-c


http://stackoverflow.com/questions/17142737/why-opencv-kdtree-always-returns-same-nearest-neighbour-in-c


http://on-demand.gputechconf.com/gtc/2013/webinar/gtc-express-itseez-opencv-webinar.pdf


http://docs.opencv.org/modules/flann/doc/flann_fast_approximate_nearest_neighbor_search.html#flann-index-t-knnsearch


https://dl.dropboxusercontent.com/u/14073242/Blog/techtidings/cvKnn/cvKnn.cpp

https://developer.nvidia.com/cuda-toolkit

Page: 94

*/