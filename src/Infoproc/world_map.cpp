#include "Infoproc/world_map.h"


world_map::world_map() : robot("NAI") {
	robot.pos.x = 200;
	robot.pos.y = 200;
	robot.pos.z = 50;

	robot.width = 300;
	robot.height = 100;

	robot.color[0] = 0.5f;
	robot.color[1] = 0.0f;
	robot.color[2] = 0.76f;
	maptodate = false;
	robot.rot = 0;
}

world_map::~world_map() {
	
}

void world_map::gentest() {

	//one unit = 100th of a foot
	//so 400 units is 4 feet
	//Creating a new plane

	//Right now all the slope dictates is color
	//0 = black
	//else = gray
	
	obj_plane newplane = obj_plane(1,1);
	newplane.p[0].x = 400; newplane.p[0].y = 799; newplane.p[0].z = 400;
	newplane.p[1].x = 400; newplane.p[1].y = 799; newplane.p[1].z = 0;
	newplane.p[2].x = 2400; newplane.p[2].y = 799; newplane.p[2].z = 400;
	newplane.p[3].x = 2400; newplane.p[3].y = 799; newplane.p[3].z = 0;
	plane_list.push_back(newplane);//Putting the plane in our list
/*
	newplane.p[0].x = 800; newplane.p[0].y = 400; newplane.p[0].z = 400;
	newplane.p[1].x = 800; newplane.p[1].y = 400; newplane.p[1].z = 0;
	newplane.p[2].x = 800; newplane.p[2].y = 2400; newplane.p[2].z = 400;
	newplane.p[3].x = 800; newplane.p[3].y = 2400; newplane.p[3].z = 0;
	plane_list.push_back(newplane);

	newplane.p[0].x = 800; newplane.p[0].y = 400; newplane.p[0].z = 400;
	newplane.p[1].x = 800; newplane.p[1].y = 400; newplane.p[1].z = 0;
	newplane.p[2].x = 800; newplane.p[2].y = 2000; newplane.p[2].z = 400;
	newplane.p[3].x = 800; newplane.p[3].y = 2000; newplane.p[3].z = 0;
	plane_list.push_back(newplane);
*/
	newplane.p[0].x = 400; newplane.p[0].y = 0; newplane.p[0].z = 400;
	newplane.p[1].x = 400; newplane.p[1].y = 0; newplane.p[1].z = 0;
	newplane.p[2].x = 400; newplane.p[2].y = 800; newplane.p[2].z = 400;
	newplane.p[3].x = 400; newplane.p[3].y = 800; newplane.p[3].z = 0;
	plane_list.push_back(newplane);
	
}

void world_map::SetRobotAttr(obj_point set_pos, float set_ang) {
	robot.pos = set_pos;
	robot.rot = set_ang;
}

const obj_cube *world_map::GetRobot() const {
	return &robot;
}

const std::vector<obj_plane> &world_map::GetPlanes() const {
	return plane_list;
}

void world_map::addobjs(std::vector<obj_base> &add_obj) {
	
}

