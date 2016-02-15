#ifndef NAI_STATE_TEST
#define NAI_STATE_TEST
#include "Infoproc/states/base_state.h"
#include "Memory/path_finding.h"
class test_state : public base_state {

	public:
		test_state(const world_map*, path_finding &);
		~test_state();
		bool Process();
		base_state *endstate();

	private:
		path_finding &pfind;



};

#endif