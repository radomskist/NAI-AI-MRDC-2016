#include "Memory/world_map.h"


world_map::world_map() {
	
}

world_map::~world_map() {
	
}

void world_map::gentest() {

	//one unit = 100th of a foot
	//so 400 units is 4 feet
	//Creating a new plane
	obj_plane newplane = obj_plane(0,0);
	newplane.x1.x = 800; newplane.x1.y = 400; newplane.x1.z = 400;
	newplane.x2.x = 800; newplane.x2.y = 400; newplane.x2.z = 0;
	newplane.y1.x = 2000; newplane.y1.y = 400; newplane.y1.z = 400;
	newplane.y2.x = 2000; newplane.y2.y = 400; newplane.y2.z = 0;
	plane_list.push_back(newplane);//Putting the plane in our list

	newplane.x1.x = 800; newplane.x1.y = 400; newplane.x1.z = 400;
	newplane.x2.x = 800; newplane.x2.y = 400; newplane.x2.z = 0;
	newplane.y1.x = 800; newplane.y1.y = 2000; newplane.y1.z = 400;
	newplane.y2.x = 800; newplane.y2.y = 2000; newplane.y2.z = 0;
	plane_list.push_back(newplane);

	newplane.x1.x = 800; newplane.x1.y = 400; newplane.x1.z = 400;
	newplane.x2.x = 800; newplane.x2.y = 400; newplane.x2.z = 0;
	newplane.y1.x = 800; newplane.y1.y = 2000; newplane.y1.z = 400;
	newplane.y2.x = 800; newplane.y2.y = 2000; newplane.y2.z = 0;
	plane_list.push_back(newplane);

	newplane.x1.x = 800; newplane.x1.y = 4000; newplane.x1.z = 400;
	newplane.x2.x = 800; newplane.x2.y = 4000; newplane.x2.z = 0;
	newplane.y1.x = 800; newplane.y1.y = 2400; newplane.y1.z = 400;
	newplane.y2.x = 800; newplane.y2.y = 2400; newplane.y2.z = 0;
	plane_list.push_back(newplane);
	
}


std::vector<obj_plane> &world_map::GetPlanes() {
	return plane_list;
}

void addobjs(std::vector<obj_base> &add_obj) {
	
}


