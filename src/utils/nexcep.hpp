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
#ifndef _NAI_UTILS_EXCEPTION
#define _NAI_UTILS_EXCEPTION
#include <exception>

struct nfail : std::exception {
	nfail(const char * set_error) {
		errorm = set_error;
	}

	const char *errorm;
	const char* what() {return errorm;}
};
#endif
