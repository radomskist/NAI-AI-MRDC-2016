#pragma once
#include "glut.h"

#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>


class Kinect
{
public:
	Kinect();
	~Kinect();
	void getFrameData(int imageType, GLubyte* dest);
	bool isInitialized() { return this->isInit; };
	bool mapColorFrameToDepthFrame(USHORT* depthData, USHORT* mappedData);
	void getDepthFrameData(USHORT* depthData);

	// CONSTANTS
	static const int IMAGE_COLOR;
	static const int IMAGE_DEPTH;

private:
	void initColor();
	void initDepth();
	bool getKinectSensor();
	void getColorFrameData(GLubyte* colorData);
	INuiFrameTexture* getNextFrame(HANDLE datastream, NUI_IMAGE_FRAME* imageFrame, NUI_LOCKED_RECT* LockedRect);
	void releaseFrame(HANDLE dataStream, INuiFrameTexture* texture, NUI_IMAGE_FRAME* imageFrame);


	INuiSensor* sensor;			// The kinect sensor
	HANDLE colorDataStream;		// The identifier of the Kinect's RGB Camera
	HANDLE depthDataStream;		// The identifier of the Kinect's RGB Camera
	int imageWidth = 640;		// Set Width of the Image Data
	int imageHeight = 480;		// Set Height of the Image Data

	long depthToRgbMap[640 * 480 * 2];

	bool isInit = false;
};
