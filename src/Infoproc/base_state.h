#ifndef NAI_STATE_BASE
#define NAI_STATE_BASE
#include "Memory/world_map.h"
#include "Memory/path_finding.h"

class base_state {

	public:
		base_state(const world_map*);
		~base_state();

		virtual bool Process() = 0; //Process information
		virtual std::string commands() = 0; //Which commands to run
		virtual base_state *endstate() = 0; //Which state to go into next?

	private:
		const world_map *wmap;

};

#endif
