#include "SDL.h"


// OpenGL Variables
GLuint textureId;								// ID of the texture to contain Kinect RGB Data
GLubyte data[FRAME_WIDTH * FRAME_HEIGHT * 4];	// BGRA array containing the texture data

// Kinect variables
HANDLE rgbStream;								// The identifier of the Kinect's RGB Camera
HANDLE depthStream;								// The identifier of the Kinect's Depth Camera
INuiSensor* sensor;								// The kinect sensor
SDL_Window *window;								// The application window


long depthToRgbMap[FRAME_WIDTH * FRAME_HEIGHT * 2];

double LuvData[FRAME_WIDTH * FRAME_HEIGHT * 3]; // double arrary containg the Luv data for each puxel



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
	if (lockedRect.Pitch != 0)
	{
		const BYTE* curr = (const BYTE*) lockedRect.pBits;
		const BYTE* dataEnd = curr + (FRAME_WIDTH * FRAME_HEIGHT) * 4;

		while (curr < dataEnd) {
			*dest++ = *curr++;
		}
	}

	// Unlock and release the frame
	texture->UnlockRect(0);
	sensor->NuiImageStreamReleaseFrame(rgbStream, &imageFrame);
}

//double calculateDistance(Point *p1, Point *p2) {
//	int p1Start = (FRAME_WIDTH * p1->y + p1->x) * 3; // Staring byte of point 1 pixel data
//	int p2Start = (FRAME_WIDTH * p2->y + p2->x) * 3; // Starting byte of point 2 pixel data

//	double l = pow((double) LuvData[p1Start] - (double) LuvData[p2Start], 2);
//	double u = pow((double) LuvData[p1Start + 1] - (double) LuvData[p2Start + 1], 2);
//	double v = pow((double) LuvData[p1Start + 2] - (double) LuvData[p2Start + 2], 2);

//	return sqrt(l + u + v);
	/* Distance approximation equation
	double rmean = (data[p1] + data[p2]) / 2;

	double l = data[p1] - data[p2];
	double u = data[p1 + 1] - data[p2 + 1];
	double v = data[p1 + 2] - data[p2 + 2];

	return pow((((512.0 + rmean) * l * l) >> 8) + 4.0 * u * u + (((767.0 - rmean) * v * v) >> 8), 0.5);
	*/
//}

//http://stackoverflow.com/questions/159590/way-to-go-from-recursion-to-iteration
//http://en.wikipedia.org/wiki/Flood_fill


void drawKinectData() {
	glBindTexture(GL_TEXTURE_2D, textureId);
	getRgbData(data);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, FRAME_WIDTH, FRAME_HEIGHT, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*) data);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(FRAME_WIDTH, 0, 0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(FRAME_WIDTH, FRAME_HEIGHT, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, FRAME_HEIGHT, 0.0f);
	glEnd();
}

GLuint get_number(GLubyte const* ptr) {
	// magic numbers for shift sizes are safe because GL types have fixed sizes
	// (8 for GLubyte and 32 for GLuint)
	return ptr[0] << 16 + ptr[1] << 8 + ptr[2];
}

char* getPointRGB(int x, int y) {
	char* rgb[3];
	int start = (FRAME_WIDTH * y + x) * 4;
	//BGRA
	_itoa_s(get_number(&data[start + 2]), rgb[0], 1, 10);
	_itoa_s(get_number(&data[start + 1]), rgb[1], 1, 10);
	_itoa_s(get_number(&data[start]), rgb[2], 1, 10);

	int color = (data[start + 2] << 16) | (data[start + 1] << 8) | data[start];
	
	return *rgb;
}

void execute() {
	SDL_Event ev;
	bool running = true;
	while (running) {
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT) running = false;
			if (ev.type == SDL_MOUSEBUTTONDOWN) {
				if (ev.button.button == SDL_BUTTON_RIGHT) {
					//detectObject = false;
				}
				else if (ev.button.button == SDL_BUTTON_LEFT) {
					//Get the mouse offsets
					//clickedX = ev.motion.x;
					//clickedY = ev.motion.y;
					//detectObject = true;
					//imageClicked(x, y);
				}
			}
		}
		drawKinectData();
		//if (detectObject) {
			//SDL_Delay(10000);
		//}
		SDL_GL_SwapWindow(window);
	}
}

void getXYZ(GLubyte *imageData, int start, double(&XYZ)[3]) {
	//imageData format is BGRA
	// SET X (aka R)
	// SET Y (aka G)
	// SET Z (aka B)
	XYZ[0] = (double) data[start + 2] / 255.0;
	XYZ[1] = (double) data[start + 1] / 255.0;
	XYZ[2] = (double) data[start] / 255.0;
}

void multiplyMatrices(double(&m1)[3][3], double(&m2)[3][1], double(&result)[3][1]) {
	for (int i = 0; i < 3; i++) {
		double sum = 0;
		for (int k = 0; k < 3; k++) {
			sum += m1[i][k] * m2[k][0];
		}
		result[i][0] = sum;
	}
}

void imageClicked(int x, int y) {
	std::cout << "Mouse Click" << "\tX:" << x << "\tY:" << y << std::endl;

	//getLuv(x, y);
}

void getLuv(int x, int y) {
	double XYZ[3];
	getXYZ(data, (FRAME_WIDTH * y + x) * 4, XYZ);
	//std::cout << "RGB: " << XYZ[0] << "," << XYZ[1] << "," << XYZ[2] << std::endl;

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

	//Muliply by Matrix
	multiplyMatrices(matrix, colorMatrix, result);
	//std::cout << "X: " << result[0][0] << "\t Y: " << result[1][0] << "\t Z: " << result[2][0] << std::endl;


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
	double u = 13 * L * (u_prime - u_prime_ref);
	double v = 13 * L * (v_prime - v_prime_ref);

	// Store Luv to pixeldata

	int start = (FRAME_WIDTH * y + x) * 3;
	LuvData[start] = L;
	LuvData[start + 1] = u;
	LuvData[start + 2] = v;
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
	// Initialize textures
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, FRAME_WIDTH, FRAME_HEIGHT,
		0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*) data);
	glBindTexture(GL_TEXTURE_2D, 0);

	// OpenGL setup
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);
	glEnable(GL_TEXTURE_2D);

	// Camera setup
	glViewport(0, 0, FRAME_WIDTH, FRAME_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, FRAME_WIDTH, FRAME_HEIGHT, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}