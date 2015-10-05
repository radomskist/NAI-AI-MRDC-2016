#include "main.h"
#include "glut.h"
#include "Kinect.h"
#include "Point.h"
#include "Plane.h"
#include <Windows.h>
#include <Ole2.h>
#include <iostream>
#include <math.h>

#define COLOR 1
#define DEPTH 0

bool initKinect();
bool initKinectColor();
bool initKinectDepth();
void getKinectData(GLubyte* dest);
void getKinectDataColor(GLubyte* dest);
void getKinectDataDepth(GLubyte* dest);



// OpenGL Variables
GLuint textureId;							// ID of the texture to contain Kinect RGB Data
GLubyte colorData[WIDTH * HEIGHT * 4];		// BGRA array containing the texture data
USHORT depthData[WIDTH * HEIGHT];			// depth data
USHORT mapData[WIDTH * HEIGHT];

Kinect* kinect;


bool initKinect() {
	kinect = new Kinect();
	return kinect->isInitialized();
}

void drawKinectData() {
	// Get Data From the Kinect
	kinect->getFrameData(Kinect::IMAGE_COLOR, colorData);
	kinect->getDepthFrameData(depthData);
	if(kinect->mapColorFrameToDepthFrame(depthData, mapData)) objectSegmentation();

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

//GLubyte data[WIDTH * HEIGHT * 4];
void objectSegmentation() {
	int x, y;
	// select 3 random points
	Point p1, p2, p3;

	//x = rand() % WIDTH;
	//y = rand() % HEIGHT;
	//x = 80-250;
	//y = 50-290;
	x = 315;
	y = 225;
	p1 = Point(x, y, getDistanceOfPixel(x,y));

	//x = rand() % WIDTH;
	//y = rand() % HEIGHT;
	x = 325;
	y = 220;
	p2 = Point(x, y, getDistanceOfPixel(x, y));

	//x = rand() % WIDTH;
	//y = rand() % HEIGHT;
	x = 320;
	y = 230;
	p3 = Point(x, y, getDistanceOfPixel(x, y));

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
				colorData[offset] = 0;		// B
				colorData[offset+1] = 0;	// G
				colorData[offset+2] = 0;	// R
			}
			/*else {	// Set to White
				colorData[offset] = 255;
				colorData[offset + 1] = 255;
				colorData[offset + 2] = 255;
			}*/
		}
	}
	std::cout << sum / tot << "\t";

	x = p1.getX();
	y = p1.getY();
	colorData[((y * WIDTH) + x) * 4] = 0;		// B
	colorData[((y * WIDTH) + x) * 4 + 1] = 255;	// G
	colorData[((y * WIDTH) + x) * 4 + 2] = 255;	// R

	x = p2.getX();
	y = p2.getY();
	colorData[((y * WIDTH) + x) * 4] = 0;		// B
	colorData[((y * WIDTH) + x) * 4 + 1] = 255;	// G
	colorData[((y * WIDTH) + x) * 4 + 2] = 255;	// R

	x = p3.getX();
	y = p3.getY();
	colorData[((y * WIDTH) + x) * 4] = 0;		// B
	colorData[((y * WIDTH) + x) * 4 + 1] = 255;	// G
	colorData[((y * WIDTH) + x) * 4 + 2] = 255;	// R

}

int getDistanceOfPixel(int x, int y) {
	int offset = (y * WIDTH) + x;

	return depthData[offset];
}