/*****************************************************************
	* Author: Steven Radomski radomskist@yahoo.com
	*
	* Copyright (C) 2017 Steven Radomski
	* 
	* This file was part of the 2016 NIU robotics AI robot project
	* 
	* This code cannot be copied or used without the permission of
	* the author
	*
	*
*****************************************************************/
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

inline unsigned int GetSec()
	{

	timeval tv;
	gettimeofday(&tv, 0);

	return tv.tv_sec;
	}

inline unsigned int GetMilli()
	{

	timeval tv;
	gettimeofday(&tv, 0);

	return (1000*tv.tv_sec) + (tv.tv_usec * 0.001);
	}

#endif
