#pragma once
#define WIDTH 640
#define HEIGHT 480
#define DISTANCE_THRESHOLD 15.00

void drawKinectData();
void objectSegmentation();
int getDistanceOfPixel(int x, int y);