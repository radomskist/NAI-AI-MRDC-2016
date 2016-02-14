#include "Infoproc/states/test_state.h"

test_state::test_state(const world_map* set_map) : base_state(set_map) {



}

bool test_state::Process() {
	commlist = "MV 90!";
	return true;
}

base_state *test_state::endstate() {
	return NULL;
}

test_state::~test_state() {


}

