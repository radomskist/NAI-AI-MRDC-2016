#ifndef NAI_DRIVE_MANAGER
#define NAI_DRIVE_MANAGER
#include "Devices/chipcomm.h"
#include "utils/ntime.hpp"
#include "Memory/path_finding.h"

/*
	* This manages all the driving of the actual robot
	* and connection to arduino
*/

class drive_man {
	public:
		drive_man(const path_finding *);
		~drive_man();
		void tick();
		bool runcom(std::string&);
		void SetChecks(bool,bool,bool);

	private:
		inline bool movecheck(std::string&);
		inline bool checkpath();


		ccomm *drivechip; /*Arduino chip*/
		bool front,right,left;
		unsigned int delay;
		std::string override;
		std::string currentpath;
		int dir;
		int currentnode;
		const path_finding *pfind;
};

#endif
