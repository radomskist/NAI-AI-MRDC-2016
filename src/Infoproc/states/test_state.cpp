#include "Infoproc/states/test_state.h"

test_state::test_state(const world_map* set_map, path_finding &set_find) : base_state(set_map), pfind(set_find) {
	
}

bool test_state::Process() {
	if(pfind.GetPath().empty())
		pfind.gotopoint(obj_point(800,800,50));

	return true;
}

base_state *test_state::endstate() {
	return NULL;
}

test_state::~test_state() {


}

