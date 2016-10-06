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
#ifndef NAI_CHIPCOMM
#define NAI_CHIPCOMM

#include "utils/nexcep.hpp"

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

/*This is a class to manage any communication to external devices through serial*/
class ccomm {
	public:
		ccomm(int);
		~ccomm();

		bool writecom(std::string&);
		std::string readall();
		std::string GetName();

	private:
		int file;
		struct termios termop;
		std::string name;
};

/*This will create a new communication class, effectively opening a new communication*/
namespace naicom { 
ccomm *createcomm(std::string); //simple factory function for creating webcams
//void listwebcams(); maybe in the future
}


#endif
