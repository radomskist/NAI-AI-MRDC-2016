#ifndef NAI_STATE_BASE
#define NAI_STATE_BASE
#include "Infoproc/world_map.h"

class base_state {

	public:
		base_state(const world_map*);
		~base_state();

		virtual bool Process() = 0; //Process information
		std::string &commands(); //Which commands to run
		virtual base_state *endstate() = 0; //Which state to go into next?
		virtual void SetStat(bool) {};

	protected:
		std::string commlist;
		const world_map *wmap;
};

#endif
