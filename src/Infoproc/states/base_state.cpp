#include "Infoproc/states/base_state.h"

base_state::base_state(const world_map *set_map) : wmap(set_map) {


}

std::string &base_state::commands() {
	return commlist;
}

base_state::~base_state() {



}
