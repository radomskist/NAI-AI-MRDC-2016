#ifndef NAI_DRIVE_MANAGER
#define NAI_DRIVE_MANAGER
#include "Devices/chipcomm.h"
#include "utils/ntime.hpp"
#include "Infoproc/path_finding.h"
#include "Infoproc/objects.hpp"

#include <sstream>
/*
	* This manages all the driving of the actual robot
	* and connection to arduino
*/

class drive_man {
	public:
		drive_man(const path_finding *, const obj_cube *);
		~drive_man();
		int tick(); //returns if doing a path
		bool runcom(std::string&);
		void override(bool);
		const std::string GetCurComm(); //get current command;

		bool GetEst(obj_point &, float &);

	private:
		inline bool movecheck(std::string&);
		inline bool obstvoid(); //obsticalavoid
		inline void execcom(); //execute command
		inline void execcom(std::string &);
		inline bool doorcheck();
		//inline void execcom(std::string&,); //execute command without rest of class

		ccomm *drivechip; /*Arduino chip*/
		const path_finding *pfind; /*path find object*/
		const obj_cube *robot; /*nai robot*/
		unsigned int delay, delaytime; //Delay between sending commands

		/*commands*/
		std::string currentpath;
		std::vector<std::string> commandhist;

		/*path info*/
		float dir;
		unsigned int cpathid;
		int currentnode; //Current node we're in

		/*override*/
		bool overridemode;
		std::string overridecom;

		/*estimation*/
		bool est;
		float turntol; //Tolerance for turning degree
		float drivespeed, turnspeed; //Approximately how fast we move
		float estiangle;
		int difference; //difference between delay and milliseconds
		obj_point estimv;

};

#endif
