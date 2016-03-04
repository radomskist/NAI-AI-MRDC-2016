#ifndef NAI_STATE_ARG_SCAN
#define NAI_STATE_ARG_SCAN
#include <iostream>
#include <string>
#include <sstream>
#include "Infoproc/world_map.h"
#include "Infoproc/states/argstate/arg_base.h"

/*arguments
a:#; 
	angle, radian. Turns until it's facing this direction scanning all areas in between. 
	If it's the same as the current direction or isn't specified it does a 360 scan.
	Always positive (if you passive negative it'll be casted to positive)
d:(l or r); 
	direction, left or right (l or r). It turns this direction till it's facing the angle. If not specified defaults to right
o:(1 or 0); 
	One scan, true or false. Whether or not it should just scan the direction or scan the areas between (one for only the direction)
f:obj details; 
	d = QR code (do QR scan)
	Find, TODO: commands b = ball, d = door qr code, s = dispenser, b = score bin
*/

class scan_state : public arg_state {

	public:
		scan_state(const world_map*, std::string);
		~scan_state();

		int Process(); //Process information
		void SetStat(std::string);

	protected:
		bool once; //only scan the dir
		bool turndon;
		float angle; //angle to turn to relative to current rot in radians
		float direction; //true for right, false for left
		std::string scandir;
};

#endif
