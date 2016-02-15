#ifndef NAI_DRIVE_MANAGER
#define NAI_DRIVE_MANAGER
#include "Devices/chipcomm.h"
#include "utils/ntime.hpp"
#include "Memory/path_finding.h"
#include "Memory/objects.hpp"
/*
	* This manages all the driving of the actual robot
	* and connection to arduino
*/

class drive_man {
	public:
		drive_man(const path_finding *, const obj_cube *);
		~drive_man();
		void tick();
		bool runcom(std::string&);
		void SetChecks(bool,bool,bool);
		void GetEst(obj_point &, float &);

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
		const obj_cube *robot;
		const path_finding *pfind;

		float estiangle;
		obj_point estimv;
};

#endif
