#ifndef NAI_STATE_OPEN_DOOR
#define NAI_STATE_OPEN_DOOR
#include "Infoproc/world_map.h"
#include "Infoproc/path_finding.h"
#include "Infoproc/states/base_state.h"

class open_door : public base_state {

	public:
		open_door(const world_map*, path_finding&);
		~open_door();

		int Process();
		void SetStat(std::string);

	protected:
		path_finding &pfind;
};

#endif
