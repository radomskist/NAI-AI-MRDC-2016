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
