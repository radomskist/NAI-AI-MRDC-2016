#ifndef NAI_STATE_BASE
#define NAI_STATE_BASE
#include "Infoproc/world_map.h"

class base_state {

	public:
		base_state(const world_map*);
		~base_state();

		virtual int Process() = 0; //Process information
		int IsExit();
		std::string &commands(); //Which commands to run
		/*commands: 
			DRIVING: 
				RA@ # = rotate
					@ = + or -, + = left, - = right
					# = degree in radians
				MV # D = move
					# = angle in radians to drive at (relative to robot. 1.57 = forward)
					D = Distance to move
			STATES:
				S = State to execute next

				SS @ = Scan State
					@ = scan attributes (look at scan_state.h)

			OTHER:
				RQ: Read QR code
				F @ #:  Find
						@ = Shape
						# = color (hue)
		*/

		virtual base_state *endstate(); //Which state to go into next?
		virtual void SetStat(std::string) {};

	protected:
		bool comred;
		std::string commlist;
		const world_map *wmap;
		int sexit;
};

#endif
