#ifndef NAI_CHIPCOMM
#define NAI_CHIPCOMM

#include "utils/nexcep.hpp"

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

class ccomm {
	public:
		ccomm(int);
		~ccomm();

		std::string readall();
		std::string GetName();

	private:
		int file;
		struct termios termop;
		std::string name;
};

namespace naicom { 
ccomm *createcomm(std::string); //simple factory function for creating webcams
//void listwebcams(); maybe in the future
}


#endif
