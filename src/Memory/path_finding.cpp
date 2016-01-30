#include "Memory/path_finding.h"

path_finding::path_finding(world_map *setmap) {
	wmap = setmap;
	start.x = 200;
	start.y = 200;

}
path_finding::~path_finding() {
	

}

std::vector<obj_point> path_finding::coarsepathfind(obj_point) {



}

std::vector<obj_point> path_finding::gotopoint(obj_point findpoint) {
	std::vector<obj_point> returnpoint;
	returnpoint.push_back(start);
	//Don't touch above


	//Testpoint
	obj_point newp;
	newp.x = 200.0f;
	newp.y = 2000.0f;
	returnpoint.push_back(newp);
	newp.x = 2000.0f;
	newp.y = 2000.0f;
	returnpoint.push_back(newp);

	//Don't touch below
	return returnpoint;
}

