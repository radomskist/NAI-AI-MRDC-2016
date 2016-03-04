#ifndef NAI_STATE_OP_DIS
#define NAI_STATE_OP_DIS
#include "Infoproc/world_map.h"
#include "Infoproc/path_finding.h"
#include "Infoproc/states/base_state.h"

class operate_dis : public base_state {

	public:
		operate_dis(const world_map*, path_finding&);
		~operate_dis();

		int Process(); //Process information
		base_state *endstate(); //Which state to go into next?
		void SetStat(std::string);

	protected:
		path_finding &pfind;
};

#endif
