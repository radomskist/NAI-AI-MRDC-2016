#include "Infoproc/test_state.h"

test_state::test_state(const world_map* set_map) : base_state(set_map) {



}

bool test_state::Process() {
	return true;
}

std::string test_state::commands() {
	return std::string("RR 5");
	
}

base_state *test_state::endstate() {
	return NULL;
}

test_state::~test_state() {


}

