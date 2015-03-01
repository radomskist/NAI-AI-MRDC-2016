#pragma once

#include <Windows.h>
#include <Ole2.h>
#include <iostream>
#include <math.h>
#include <bitset>
#include <chrono>
#include <stdexcept>

// OpenCV Libraries
#include <opencv2/core/core.hpp>
#include <opencv2/flann/flann.hpp>

// Custom Libraries
#include "glut.h"
#include "Kinect.h"
#include "Point.h"
#include "Plane.h"


#define WIDTH 640
#define HEIGHT 480
#define DISTANCE_THRESHOLD 15.00


#define COLOR 1
#define DEPTH 0
#define RUN_SEARCH_RADIUS 1
#define RUN_SEARCH_NEAREST_NEIGHBOR 0

#define NUM_NEAREST_NEIGHBORS 20
#define SEARCH_RADIUS 20
#define MAX_LOOPS 2000

#define POINTS_IN_CLOUD (WIDTH * HEIGHT)
#define NUM_SEARCHES 5000
#define NUM_SEARCH_TREES 1
#define MAX_RESULTS (SEARCH_RADIUS * SEARCH_RADIUS)

//#define MAX_RESULTS 200


void getKinectData(GLubyte* dest);
void getKinectDataColor(GLubyte* dest);
void getKinectDataDepth(GLubyte* dest);
void drawKinectData();
void objectSegmentation();

int getDistanceOfPixel(int x, int y);

bool initKinect();
bool initKinectColor();
bool initKinectDepth();
