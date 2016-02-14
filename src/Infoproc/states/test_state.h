#ifndef NAI_STATE_TEST
#define NAI_STATE_TEST
#include "Infoproc/states/base_state.h"

class test_state : public base_state {

	public:
		test_state(const world_map*);
		~test_state();
		bool Process();
		std::string &commands();
		base_state *endstate();

	private:




};

#endif
