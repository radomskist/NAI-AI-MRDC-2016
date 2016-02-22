#include "Infoproc/world_map.h"

#include <iostream>
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
	robot.rot = 1.6;
}

void world_map::AddPlanes(std::vector<obj_plane> &setplanes) {

	//Translating the plane from local to world coordinates
	//Points 1 and 2 are basically ignored
	for(int j = 0; j < setplanes.size(); j++) {
		for(int i = 0; i < 2; i++) {
			float plx = setplanes[j].p[i*3].x * 0.32808399; //dist forward converted from mm to 1/100 of a foot
			float ply = setplanes[j].p[i*3].y * 0.32808399; //dist from center of cam

			float sang = sin(1.57);
			float cang = cos(1.57);

			//Applying rotation matrix
			float nplx = robot.pos.x + plx*cang - ply*sang;
			float nply = robot.pos.y + plx*sang + ply*cang;

			//Setting 0 and 3
			setplanes[j].p[i*3].x = nplx;
			setplanes[j].p[i*3].y = nply;

		}

		//If too small skip (due to errors)
		if(abs(abs(setplanes[j].p[0].x + setplanes[j].p[0].y) - abs(setplanes[j].p[3].x + setplanes[j].p[3].y)) < 200)
			continue;
		
		/*Lining up to grid*/
		setplanes[j].p[0].x = ((int)(setplanes[j].p[0].x*.0025)) * 400;
		setplanes[j].p[0].y = ((int)(setplanes[j].p[0].y*.0025)) * 400;
		setplanes[j].p[3].x = ((int)(setplanes[j].p[3].x*.0025)) * 400;
		setplanes[j].p[3].y = ((int)(setplanes[j].p[3].y*.0025)) * 400;

		setplanes[j].p[1] = setplanes[j].p[0];
		setplanes[j].p[1].z = 0;
		setplanes[j].p[2] = setplanes[j].p[3];
		setplanes[j].p[3].z = 0;
		
		/*TODO break up into plane per grid*/


		/*checking if plane exists*/
		bool match = false;
		for(int k = 0; k < plane_list.size(); k++) {
			if(abs(plane_list[k].p[0].x - setplanes[k].p[0].x) > 10)
				continue;

			if(abs(plane_list[k].p[0].y - setplanes[k].p[0].y) > 10)
				continue;

			/*TODO: Count how many times this plane is matched*/
			match = true;
			break;
		}

		if(!match)
			plane_list.push_back(setplanes[j]);
	}
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


