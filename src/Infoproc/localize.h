#ifndef NAI_LOCALIZER
#define NAI_LOCALIZER
#include "Infoproc/world_map.h"
#include "Infoproc/driveman.h"

class localizer {
	public:
		localizer(drive_man &, const obj_cube *);
		~localizer();

		/*Approximating based on info*/
		bool approximate(obj_point&, float&);

		/*run when dead end*/
		void lost();
		
	private:
		const obj_cube *robot;
		drive_man &driveman;
		

};

#endif
