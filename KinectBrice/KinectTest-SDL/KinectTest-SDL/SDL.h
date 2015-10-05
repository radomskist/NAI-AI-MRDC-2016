#pragma once

#include <Ole2.h>

#include <cmath>
#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#define GL_GLEXT_PROTOTYPES
#include <SDL.h>
#include <SDL_opengl.h>

#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>

#define FRAME_WIDTH 640
#define FRAME_HEIGHT 480
#define WAIT_FOR_FRAME_MS 0
#define MAX_COLOR_DISANCE 0.2

// OpenGL Variables
extern GLuint textureId;								// ID of the texture to contain Kinect RGB Data
extern GLubyte data[FRAME_WIDTH * FRAME_HEIGHT * 4];	// BGRA array containing the texture data

// Kinect variables
extern HANDLE rgbStream;								// The identifier of the Kinect's RGB Camera
extern HANDLE depthStream;								// The identifier of the Kinect's Depth Camera
extern INuiSensor* sensor;								// The kinect sensor
extern SDL_Window *window;								// The application window

extern long depthToRgbMap[FRAME_WIDTH * FRAME_HEIGHT * 2];


extern double LuvData[FRAME_WIDTH * FRAME_HEIGHT * 3]; // double arrary containg the Luv data for each puxel

bool initKinect();
void getDepthData(GLubyte* dest);
void getRgbData(GLubyte* dest);
void drawKinectData();
GLuint get_number(GLubyte const* ptr);
char* getPointRGB(int x, int y);
void execute();
void getXYZ(GLubyte *imageData, int start, double(&XYZ)[3]);
void multiplyMatrices(double(&m1)[3][3], double(&m2)[3][1], double(&result)[3][1]);
void imageClicked(int x, int y);
void getLuv(int x, int y);
bool init(int argc, char* argv []);
void openGLSetup();