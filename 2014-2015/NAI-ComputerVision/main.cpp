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

float loop = 0;
void drawKinectData() {
	// Get Data From the Kinect
	kinect->getFrameData(Kinect::IMAGE_COLOR, colorData);
	kinect->getDepthFrameData(depthData);
	if (kinect->mapColorFrameToDepthFrame(depthData, mapData) && loop++ > 10) {
		objectSegmentation();
	}

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
	//pointCloud.reserve(WIDTH * HEIGHT);
	//for (int x = 0; x < HEIGHT; x++) {
		//for (int y = 0; y < WIDTH; y++) {
			//pointCloud.push_back(cv::Point3f());
		//}
	//}
	if (!initKinect()) return 99999;


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

// get a CV_8U matrix from a Kinect depth frame 
cv::Mat *GetDepthImage(USHORT *depthData, int width, int height) {
	const int imageSize = width * height;
	cv::Mat *out = new cv::Mat(height, width, CV_8U);
	// map the values to the depth range
	for (int i = 0; i < imageSize; i++) {
		// get the lower 8 bits
		USHORT depth = depthData[i];
		//if (depth >= kLower && depth <= kUpper)
		//{
			//float y = c * (depth - kLower);
			//out->at<byte>(i) = (byte) y;
		//}
		//else
		//{
			out->at<byte>(i) = 0;
		//}
	}
	return out;
}

// get a CV_8UC4 (RGB) Matrix from Kinect RGB frame
cv::Mat *GetColorImage(unsigned char *bytes, int width, int height) {
	const unsigned int img_size = width * height * 4;
	cv::Mat *out = new cv::Mat(height, width, CV_8UC4);

	// copy data
	memcpy(out->data, bytes, img_size);

	return out;
}

void applyGaussianBlur(GLubyte *bytes) {
	const unsigned int img_size = WIDTH * HEIGHT * 4; // Calculate amount of size required
	cv::Mat mat(HEIGHT, WIDTH, CV_8UC4); // Create a Mat
	cv::Mat dest(HEIGHT, WIDTH, CV_8UC3);
	memcpy(mat.data, bytes, img_size); // Copy the data

	cv::cvtColor(mat, mat, CV_BGRA2BGR);
	// Apply the blur
	//cv::GaussianBlur(mat,
		//mat,
		//cv::Size(3, 3),
		//3);

	cv::medianBlur(mat, mat, 3);
	cv::bilateralFilter(mat, dest, 7, 5, 5);
	cv::cvtColor(dest, mat, CV_BGR2BGRA);

	memcpy(bytes, mat.data, img_size);
}


//GLubyte data[WIDTH * HEIGHT * 4];
void objectSegmentation() {
	// Start Clock to measure time method takes
	auto start = std::chrono::high_resolution_clock::now();

	applyGaussianBlur(colorData);

	// Populate the point cloud
	ObjectSegmentor<WIDTH, HEIGHT> segmentor(colorData, depthData);
	segmentor.findObjects();

	std::cout << "Buckets Found: " << segmentor.numberOfBuckets() << std::endl;

	std::vector<cv::Point3f> points;
	
	cv::Point3f p;
	int r, g, b;
	for (int i = 0; i < segmentor.buckets.size(); i++) {
		points = segmentor.buckets[i].points;
		r = rand() % 256;
		g = rand() % 256;
		b = rand() % 256;
		for (int j = 0; j < points.size(); j++) {
			p = points[j];

			colorData[(((int) p.y * WIDTH) + (int) p.x) * 4] = b;		// B
			colorData[(((int) p.y * WIDTH) + (int) p.x) * 4 + 1] = g;	// G
			colorData[(((int) p.y * WIDTH) + (int) p.x) * 4 + 2] = r;	// R
		}
	}

	// Get the total time in ms the method took
	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	long long microseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

	// Print out the elpsed time of the method
	std::cout << "Elapsed Time: " << microseconds << " ms\n";
}



int getDistanceOfPixel(int x, int y) {
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