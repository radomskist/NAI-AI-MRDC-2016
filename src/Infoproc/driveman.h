#ifndef NAI_DRIVE_MANAGER
#define NAI_DRIVE_MANAGER
#include "Devices/chipcomm.h"

/*
	* This manages all the driving of the actual robot
	* and connection to arduino
*/

class drive_man {
	public:
		drive_man();
		~drive_man();
		bool runcom(std::string&) const;
		bool movecheck(std::string&) const;
		void SetChecks(bool,bool,bool);

	private:
		ccomm *drivechip; /*Arduino chip*/
		bool front,right,left;

};

#endif
