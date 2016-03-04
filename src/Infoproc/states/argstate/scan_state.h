#ifndef NAI_STATE_ARG_SCAN
#define NAI_STATE_ARG_SCAN
#include <iostream>
#include <string>
#include "Infoproc/world_map.h"
#include "Infoproc/states/argstate/arg_base.h"
class scan_state : public arg_state {

	public:
		scan_state(const world_map*, std::string);
		~scan_state();

		bool Process(); //Process information

	protected:
};

#endif
