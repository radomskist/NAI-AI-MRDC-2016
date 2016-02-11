#ifndef _NAI_UTILS_TIME
#define _NAI_UTILS_TIME

/****************************************************************
   PROGRAM:		NaiTime Library
   AUTHOR:		Steven Radomski
   LOGON ID:	Z1783380 
   DATE:		October, 8 2015

   NOTES:		Contains convinient time functions for the NAI
				robot.

****************************************************************/
#include <sys/time.h>

/*Get milli of second
Basically how many milliseconds
occured in the current second*/
inline int GetMilli()
	{

	timeval tv;
	gettimeofday(&tv, 0);

	return tv.tv_usec * 1000;
	}

inline int GetSec()
	{

	timeval tv;
	gettimeofday(&tv, 0);

	return tv.tv_sec;
	}

#endif
