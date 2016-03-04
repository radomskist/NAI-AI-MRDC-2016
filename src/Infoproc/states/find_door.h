#ifndef NAI_STATE_FIRST_DOOR
#define NAI_STATE_FIRST_DOOR
#include "Infoproc/world_map.h"
#include "Infoproc/states/base_state.h"
class first_door : public base_state {

	public:
		first_door(const world_map*);
		~first_door();

		int Process();
		void SetStat(std::string);
	protected:
};

#endif
