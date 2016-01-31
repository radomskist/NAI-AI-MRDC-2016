#include "Memory/path_finding.h"

path_finding::path_finding(world_map *setmap) {
	wmap = setmap;
}

path_finding::~path_finding() {
	

}

std::vector<obj_point> path_finding::coarsepathfind(obj_point) {



}

std::vector<obj_point> path_finding::gotopoint(obj_point findpoint) {
	std::vector<obj_point> returnpoint;
	obj_point start = wmap->GetRobot().pos;
	returnpoint.push_back(start);
	obj_point newp;
	newp.z = 50.0f;
	//Don't touch above


	//Testpoint
	newp.x = 200.0f;
	newp.y = 2200.0f;
	returnpoint.push_back(newp);

	//Don't touch below
	newp.x = findpoint.x;
	newp.y = findpoint.y;
	returnpoint.push_back(newp);
	return returnpoint;
}

