#include <Windows.h>
#include <Ole2.h>
#include <GL/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include <cmath>
#include <cstdio>

#define GL_GLEXT_PROTOTYPES
#include <SDL.h>
#include <SDL_opengl.h>

#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>


#define FRAME_WIDTH 640
#define FRAME_HEIGHT 480
#define WAIT_FOR_FRAME_MS 0

// OpenGL Variables
GLuint textureId;								// ID of the texture to contain Kinect RGB Data
GLuint vboId;									// Vertex buffer ID
GLuint cboId;									// Color buffer ID
GLubyte data[FRAME_WIDTH * FRAME_HEIGHT * 4];	// BGRA array containing the texture data

// Kinect variables
HANDLE rgbStream;								// The identifier of the Kinect's RGB Camera
HANDLE depthStream;								// The identifier of the Kinect's Depth Camera
INuiSensor* sensor;								// The kinect sensor
SDL_Window *window;								// The application window

long depthToRgbMap[FRAME_WIDTH * FRAME_HEIGHT * 2];
//float colorarray[FRAME_WIDTH * FRAME_HEIGHT * 3];
//float vertexarray[FRAME_WIDTH * FRAME_HEIGHT * 3];


bool initKinect() {
	// Get a working kinect sensor
	int numSensors;
	if (NuiGetSensorCount(&numSensors) < 0 || numSensors < 1) return false;
	if (NuiCreateSensorByIndex(0, &sensor) < 0) return false;

	// Initialize senso
	sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR);

	// Open Depth stream
	sensor->NuiImageStreamOpen(
		NUI_IMAGE_TYPE_DEPTH,					// Depth camera or rgb camera?
		NUI_IMAGE_RESOLUTION_640x480,			// Image resolution
		0,										// Image stream flags, e.g. near mode
		2,										// Number of frames to buffer
		NULL,									// Event handle
		&depthStream);
	// Open Color Stream
	sensor->NuiImageStreamOpen(
		NUI_IMAGE_TYPE_COLOR,					// Depth camera or rgb camera?
		NUI_IMAGE_RESOLUTION_640x480,			// Image resolution
		0,										// Image stream flags, e.g. near mode
		2,										// Number of frames to buffer
		NULL,									// Event handle
		&rgbStream);

	return sensor;
}

void getDepthData(GLubyte* dest) {
	NUI_IMAGE_FRAME imageFrame; // Struct containing all the metadata about the frame
	NUI_LOCKED_RECT lockedRect; // Pointer to the actual data

	// Check to see if a Frame is available
	if (sensor->NuiImageStreamGetNextFrame(depthStream, WAIT_FOR_FRAME_MS, &imageFrame) < 0) {
		return; // No frames available
	}

	// Lock the frame
	INuiFrameTexture* texture = imageFrame.pFrameTexture;
	texture->LockRect(0, &lockedRect, NULL, 0);

	// Copy the data from the locked frame
	if (lockedRect.Pitch != 0) {
		const USHORT* curr = (const USHORT*) lockedRect.pBits;
		float* fdest = (float*) dest;
		long* depth2rgb = (long*) depthToRgbMap;
		for (int j = 0; j < FRAME_HEIGHT; ++j) {
			for (int i = 0; i < FRAME_WIDTH; ++i) {
				// Get depth of pixel in millimeters
				USHORT depth = NuiDepthPixelToDepth(*curr);
				// Store coordinates of the point corresponding to this pixel
				Vector4 pos = NuiTransformDepthImageToSkeleton(i, j, *curr);
				*fdest++ = pos.x / pos.w;
				*fdest++ = pos.y / pos.w;
				*fdest++ = pos.z / pos.w;
				// Store the index into the color array corresponding to this pixel
				NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(
					NUI_IMAGE_RESOLUTION_640x480,	// color frame resolution
					NUI_IMAGE_RESOLUTION_640x480,	// depth frame resolution
					NULL,							// pan/zoom of color image (IGNORE THIS)
					i, j, *curr,					// Column, row, and depth in depth image
					depth2rgb, depth2rgb + 1        // Output: column and row (x,y) in the color image
					);
				depth2rgb += 2;
				*curr++;
			}
		}
	}

	// Unlock and release the frame
	texture->UnlockRect(0);
	sensor->NuiImageStreamReleaseFrame(depthStream, &imageFrame);
}

void getRgbData(GLubyte* dest) {
	NUI_IMAGE_FRAME imageFrame; // Struct containing all the metadata about the frame
	NUI_LOCKED_RECT lockedRect; // Pointer to the actual data

	// Check to see if a Frame is available
	if (sensor->NuiImageStreamGetNextFrame(rgbStream, WAIT_FOR_FRAME_MS, &imageFrame) < 0) {
		return; // No frames available
	}

	// Lock the frame
	INuiFrameTexture* texture = imageFrame.pFrameTexture;
	texture->LockRect(0, &lockedRect, NULL, 0);

	// Copy the data from the locked frame
	if (lockedRect.Pitch != 0) {
		const BYTE* start = (const BYTE*) lockedRect.pBits;
		float* fdest = (float*) dest;
		long* depth2rgb = (long*) depthToRgbMap;
		for (int j = 0; j < FRAME_HEIGHT; ++j) {
			for (int i = 0; i < FRAME_WIDTH; ++i) {
				// Determine color pixel for depth pixel i,j
				long x = *depth2rgb++;
				long y = *depth2rgb++;
				// If out of bounds, then do not color this pixel
				if (x < 0 || y < 0 || x > FRAME_WIDTH || y > FRAME_HEIGHT) {
					for (int n = 0; n < 3; ++n) *fdest++ = 0.f;
				}
				else {
					// Determine rgb color for depth pixel (i,j) from color pixel (x,y)
					const BYTE* color = start + (x + FRAME_WIDTH * y) * 4;
					for (int n = 0; n < 3; ++n) *fdest++ = color[2 - n] / 255.f;
				}
			}
		}
	}

	// Unlock and release the frame
	texture->UnlockRect(0);
	sensor->NuiImageStreamReleaseFrame(rgbStream, &imageFrame);
}

void getKinectData() {
	const int dataSize = FRAME_WIDTH * FRAME_HEIGHT * 3 * sizeof(float);
	GLubyte* ptr;
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, dataSize, 0, GL_DYNAMIC_DRAW);
	ptr = (GLubyte*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (ptr) {
		getDepthData(ptr);
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER, cboId);
	glBufferData(GL_ARRAY_BUFFER, dataSize, 0, GL_DYNAMIC_DRAW);
	ptr = (GLubyte*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (ptr) {
		getRgbData(ptr);
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void rotateCamera() {
	static double angle = 0.;
	static double radius = 3.;
	double x = radius*sin(angle);
	double z = radius*(1 - cos(angle)) - radius / 2;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, 0, z, 0, 0, radius / 2, 0, 1, 0);
	angle += 0.05;
}

void drawKinectData() {
	getKinectData();
	rotateCamera();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, cboId);
	glColorPointer(3, GL_FLOAT, 0, NULL);

	glPointSize(1.f);
	glDrawArrays(GL_POINTS, 0, FRAME_WIDTH * FRAME_HEIGHT);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void execute() {
	SDL_Event ev;
	bool running = true;
	while (running) {
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT) running = false;
		}
		drawKinectData();
		SDL_GL_SwapWindow(window);
	}
}

bool init(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Kinect Window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		FRAME_WIDTH, FRAME_HEIGHT,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	// Create an OpenGL context associated with the window.
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);

	return window;
}

void openGLSetup() {
	// OpenGL setup
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);

	// Set up array buffers
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glGenBuffers(1, &cboId);
	glBindBuffer(GL_ARRAY_BUFFER, cboId);

	// Camera setup
	glViewport(0, 0, FRAME_WIDTH, FRAME_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, FRAME_WIDTH / (GLdouble) FRAME_HEIGHT, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 0, 0, 0, 1, 0, 1, 0);
}

int main(int argc, char* argv[]) {
	if (!init(argc, argv)) return 1;
	if (!initKinect()) return 1;

	openGLSetup();

	// Main loop
	execute();


	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();

	return 0;
}