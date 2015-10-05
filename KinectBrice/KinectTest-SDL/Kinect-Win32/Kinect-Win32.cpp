// Kinect-Win32.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <NuiApi.h>
using namespace std;
int main()
{
	cout << "Hello world" << endl;
	NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON);
	NUI_SKELETON_FRAME ourframe;
	while (1)
	{
		NuiSkeletonGetNextFrame(0, &ourframe);
		for (int i = 0; i < 6; i++)
		{
			if (ourframe.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED)
				cout << "Right Hand: ";
			cout << ourframe.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].y << endl;
		}
		system("cls");
	}
	NuiShutdown();
	return 0;
}