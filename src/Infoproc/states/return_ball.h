#ifndef NAI_STATE_RETURN_BALL
#define NAI_STATE_RETURN_BALL
#include "Infoproc/world_map.h"
#include "Infoproc/states/operate_dispenser.h"
#include "Infoproc/path_finding.h"
#include "Infoproc/states/base_state.h"
#include "Infoproc/states/find_door.h"

class return_ball : public base_state {

	public:
		return_ball(const world_map*, path_finding&);
		~return_ball();

		int Process();
		void SetStat(std::string);
		base_state *endstate();
	protected:
		path_finding &pfind;
		int mode;
		const obj_cube *robot;
};

#endif
