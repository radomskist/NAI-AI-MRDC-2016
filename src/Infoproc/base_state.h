#ifndef NAI_STATE_BASE
#define NAI_STATE_BASE
#include "Memory/world_map.h"
#include "Memory/path_finding.h"

class base_state {

	public:
		base_state(const world_map&);
		~base_state();


	private:
		const world_map wmap;

};

#endif
