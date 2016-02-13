#include "Devices/chipcomm.h"

ccomm *naicom::createcomm(std::string ofname) {
	//TODO CHECK FOR NAME?
	bool reading = true;
	int file;
	int iterator = 0;

	std::string directory = "/dev/ttyACM";

	while(file == 0) {
		std::string cur_dir = directory + std::to_string(iterator);

		if((file = open(cur_dir.c_str(), O_RDWR| O_NOCTTY | O_NDELAY)) == -1){ 
			close(file);
			throw nfail("Connection could not be made.");
		    return NULL;
		}

		return new ccomm(file);

	/*TODO: Read first command for name and reloop if not matching*/
	iterator++;
	}
}


ccomm::ccomm(int set_file) {
	file = set_file;

	tcgetattr(file, &termop);//Gets the current options for the port

	/* 9600 baud */
	cfsetispeed(&termop, B115200);
	cfsetospeed(&termop, B115200);

	/*Setting options*/
	termop.c_cflag |= (CLOCAL | CREAD);
	termop.c_cflag &= ~PARENB;
	termop.c_cflag &= ~CSTOPB;
	termop.c_cflag &= ~CSIZE;
	termop.c_cflag |= CS8;

	/*Setting options*/
	tcsetattr(file, TCSANOW, &termop);
		
}


std::string ccomm::readall() {
//TODO Loop until it reads everything
	char buff[50];
	//int w = write(file, "FDS", 3);
	if(read(file, buff, 50) != -1)
		std::cout << buff << std::endl;

}

ccomm::~ccomm() {
	close(file);
}
