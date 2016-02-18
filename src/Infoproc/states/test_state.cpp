#include "Infoproc/states/test_state.h"

test_state::test_state(const world_map* set_map, path_finding &set_find) : base_state(set_map), pfind(set_find) {
	pathstat = 0;
	curpath = 0;
}

bool test_state::Process() {
	if(pathstat == 0) {
		if(pfind.gotopoint(obj_point(800,800,50))) {
			pathstat = 1;
			curpath = pfind.GetPathID();
		}
	}
	else if(pathstat == 2) {
		if(pfind.gotopoint(obj_point(200,200,50))) {
			pathstat = 3;
			curpath = pfind.GetPathID();
		}
	}

	return true;
}

void test_state::SetStat(bool set) {
	if(set && (curpath == pfind.GetPathID())) {
		std::cout << "OH BABY" << std::endl;
		curpath = 0;
		pathstat++;
	}
}


base_state *test_state::endstate() {
	return NULL;
}

test_state::~test_state() {


}

