#ifndef NAI_STATE_TEST
#define NAI_STATE_TEST
#include "Infoproc/states/base_state.h"
#include "Infoproc/path_finding.h"
class test_state : public base_state {

	public:
		test_state(const world_map*, path_finding &);
		~test_state();
		int Process();
		base_state *endstate();
		void SetStat(std::string);

	private:
		int pathstat;
		path_finding &pfind;
		unsigned int curpath;

};

#endif
