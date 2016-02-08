#ifndef _NAI_KINECTWINDOW
#define _NAI_KINECTWINDOW

/*SDL headers for windows*/
#include "Viewer/imgwin.h"
#include "utils/nimg.h"

class kinwin : public imgwin {
	public:
		kinwin(unsigned int, unsigned int, unsigned int);
		~kinwin();
		void GetKeys();
};

#endif
