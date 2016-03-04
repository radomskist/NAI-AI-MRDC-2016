#include "Infoproc/world_map.h"
#include "Infoproc/states/operate_dispenser.h"

operate_dis::operate_dis(const world_map* set_map, path_finding& set_pfind) : base_state(set_map), pfind(set_pfind)  {


}

operate_dis::~operate_dis() { }

//Process information
int operate_dis::Process() {


}
base_state *operate_dis::endstate(){ //Which state to go into next? 


}
void operate_dis::SetStat(std::string) {

}

