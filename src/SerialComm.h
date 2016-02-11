#include <stdio.h>

#include <string.h>

#include <fcntl.h>

#include <errno.h>

#include <termios.h>

#include <unistd.h>

class SerialComm{
public:

	SerialComm();
	~SerialComm();
	bool send(string);
	bool receive(string);
private:

	int fd1;

	int fd2;

	char *buff,*buffer,*bufptr;

	int wr,rd,nbytes,tries;
};
