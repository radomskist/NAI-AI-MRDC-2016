#include "Kinect.h"

// Constants
const int Kinect::IMAGE_COLOR = 1;
const int Kinect::IMAGE_DEPTH = 2;


// Kinect
Kinect::Kinect() {
	if (this->getKinectSensor()) {
		this->initColor();
		this->initDepth();
		this->isInit = true;
	}
}


Kinect::~Kinect() {
}

// Get the first working Kinect Sensor
bool Kinect::getKinectSensor() {
	if (this->sensor) return true;
	int numSensors;

	// Get the Number of Kinect Sensors
	// If less than 1 return false
	if (NuiGetSensorCount(&numSensors) < 0 || numSensors < 1) return false;

	// Create an instance of the first Kinect Sensor
	// If failed return false
	if (NuiCreateSensorByIndex(0, &this->sensor) < 0) return false;

	// Allow Color and Depth Data
	this->sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR);

	return true;
}

void Kinect::initColor() {
	// Get the first available sensor
	if (!this->getKinectSensor()) return;

	// Initialize sensor
	sensor->NuiImageStreamOpen(
		NUI_IMAGE_TYPE_COLOR,			// Depth camera or rgb camera?
		NUI_IMAGE_RESOLUTION_640x480,	// Image resolution
		0,								// Image stream flags, e.g. near mode
		2,								// Number of frames to buffer
		NULL,							// Event handle
		&this->colorDataStream);		// Stream handle
}

void Kinect::initDepth() {
	// Get the first available sensor
	if (!this->getKinectSensor()) return;

	// Initialize sensor
	sensor->NuiImageStreamOpen(
		NUI_IMAGE_TYPE_DEPTH,			// Depth camera or rgb camera?
		NUI_IMAGE_RESOLUTION_640x480,	// Image resolution
		0,								// Image stream flags, e.g. near mode (NUI_IMAGE_STREAM_FLAG_ENABLE_NEAR_MODE)
		2,								// Number of frames to buffer
		NULL,							// Event handle
		&this->depthDataStream);		// Stream handle
}


void Kinect::getFrameData(int imageType, GLubyte* dest) {
	if (imageType == Kinect::IMAGE_COLOR) {
		// Get the color frame data
		this->getColorFrameData(dest);
	}
	else if (imageType == Kinect::IMAGE_DEPTH) {
		// Get the depth frame data
		//this->getDepthFrameData(dest);
	}
}

INuiFrameTexture* Kinect::getNextFrame(HANDLE dataStream, NUI_IMAGE_FRAME* imageFrame, NUI_LOCKED_RECT* lockedRect) {
	// Get the next frame
	// Return if a frame isn't available
	if (this->sensor->NuiImageStreamGetNextFrame(dataStream, 0, imageFrame) < 0) return NULL;

	// Get a pointer to the frame texture
	INuiFrameTexture* texture = imageFrame->pFrameTexture;

	// Lock the Frame Texture
	texture->LockRect(0, lockedRect, NULL, 0);

	return texture;
}

void Kinect::releaseFrame(HANDLE dataStream, INuiFrameTexture* texture, NUI_IMAGE_FRAME* imageFrame) {
	// Unlock the Frame
	texture->UnlockRect(0);

	// Release the frame from the sensor
	sensor->NuiImageStreamReleaseFrame(dataStream, imageFrame);
}

void Kinect::getColorFrameData(GLubyte* dest) {
	NUI_IMAGE_FRAME imageFrame;
	NUI_LOCKED_RECT lockedRect;
	INuiFrameTexture* texture;

	// Get the Next Image Frame and lock the data
	texture = this->getNextFrame(this->colorDataStream, &imageFrame, &lockedRect);
	if (!texture) return;


	// Check to see if there are bytes in the LockedRect
	if (lockedRect.Pitch != 0)
	{
		// Get a reference to the start of the lockedRect data
		const BYTE* curr = (const BYTE*) lockedRect.pBits;

		// Get a reference to the end of the lockedRect data
		const BYTE* dataEnd = curr + (this->imageHeight * this->imageWidth) * 4;

		// Transfer all the data from the lockedRect to the passed destination
		while (curr < dataEnd) {
			*dest++ = *curr++;
		}
	}

	// Unlock the Image Frame
	this->releaseFrame(this->colorDataStream, texture, &imageFrame);
}

void Kinect::getDepthFrameData(USHORT* dest) {
	float* fdest = (float*) dest;
	long* depth2rgb = (long*) this->depthToRgbMap;

	NUI_IMAGE_FRAME imageFrame;
	NUI_LOCKED_RECT lockedRect;
	INuiFrameTexture* texture;

	// Get the Next Image Frame and lock the data
	texture = this->getNextFrame(this->depthDataStream, &imageFrame, &lockedRect);
	if (!texture) return;

	// Check to see if there are bytes in the LockedRect
	if (lockedRect.Pitch != 0)
	{
		const USHORT* curr = (const USHORT*) lockedRect.pBits;
		const USHORT* dataEnd = curr + (this->imageHeight * this->imageWidth);

		for (int i = 0; i < lockedRect.size; i += sizeof(USHORT)) {
			USHORT depth = NuiDepthPixelToDepth(*curr++);
			//std::cout << depth;
			*dest++ = depth;
		}
		/*
		while (curr < dataEnd) {
			// Get depth in millimeters
			USHORT depth = NuiDepthPixelToDepth(*curr++);

			*dest++ = depth;

			// Draw a grayscale image of the depth:
			// Set B,G,R, to depth%256
			// Set Alpha to 1 to make 100% visible
//			for (int i = 0; i < 3; ++i) {
//				*dest++ = (BYTE) depth % 256;
//			}
//			*dest++ = 0xff; // Set Alpha to 1
		}*/
	}

	// Unlock the Image Frame
	this->releaseFrame(this->depthDataStream, texture, &imageFrame);
}


bool Kinect::mapColorFrameToDepthFrame(USHORT* depthData, USHORT* mappedData) {
	INuiCoordinateMapper* coordMapper;
	NUI_COLOR_IMAGE_POINT* colorPoints = new NUI_COLOR_IMAGE_POINT[640 * 480]; // color points
	NUI_DEPTH_IMAGE_PIXEL* depthPoints = new NUI_DEPTH_IMAGE_PIXEL[640 * 480]; // depth pixel

	/* BE SURE THAT YOU ARE WORKING WITH THE RIGHT HEIGHT AND WIDTH */
	unsigned long refWidth = 0;
	unsigned long refHeight = 0;
	NuiImageResolutionToSize(NUI_IMAGE_RESOLUTION_640x480, refWidth, refHeight);
	int width = static_cast<int>(refWidth); //get the image width in a right way
	int height = static_cast<int>(refHeight); //get the image height in a right way

	sensor->NuiGetCoordinateMapper(&coordMapper); // get the coord mapper
	//Map your frame;
	HRESULT result = coordMapper->MapDepthFrameToColorFrame(NUI_IMAGE_RESOLUTION_640x480, width * height, depthPoints, NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, width * height, colorPoints);
	if (FAILED(result)) return false;

	// apply map in terms of x and y (image coordinates);
	for (int i = 0; i < width * height; i++) {
		if (colorPoints[i].x > 0 && colorPoints[i].x < width && colorPoints[i].y > 0 && colorPoints[i].y < height) {
			USHORT d = *(depthData + i);
			USHORT offset = colorPoints[i].x + colorPoints[i].y * width;
			*(mappedData + colorPoints[i].x + colorPoints[i].y * width) = *(depthData + i);
			USHORT val = *(mappedData + colorPoints[i].x + colorPoints[i].y * width);
		}
	}

	// free your memory!!!
	delete colorPoints;
	delete depthPoints;
	return true;
}