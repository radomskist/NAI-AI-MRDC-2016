#include "Infoproc/world_map.h"

#include <iostream>
world_map::world_map() : robot("NAI") {
	robot.pos.x = 200;
	robot.pos.y = 200;
	robot.pos.z = 50;

	robot.width = 300;
	robot.height = 100;

	//Opengl render color
	robot.color[0] = 0.5f;
	robot.color[1] = 0.0f;
	robot.color[2] = 0.76f;
	maptodate = false;
	robot.rot = 1.57;

	width = 11;
	size = width*width;

	for(int i = 0; i < size; i++) {
		grid[i].x = i % width;
		grid[i].y = i / width;
		grid[i].tags = 0;
	}
}

void world_map::AddPlanes(std::vector<obj_plane> &setplanes) {

	//Translating the plane from local to world coordinates
	//Points 1 and 2 are basically ignored
	for(int j = 0; j < setplanes.size(); j++) {
		for(int i = 0; i < 2; i++) {
			float plx = setplanes[j].p[i*3].x * 0.32808399; //dist forward converted from mm to 1/100 of a foot
			float ply = setplanes[j].p[i*3].y * 0.32808399; //dist from center of cam

			float sang = sin(robot.rot);
			float cang = cos(robot.rot);

			//Applying rotation matrix
			float nplx = robot.pos.x + plx*cang - ply*sang;
			float nply = robot.pos.y + plx*sang + ply*cang;

			//Setting 0 and 3
			setplanes[j].p[i*3].x = nplx;
			setplanes[j].p[i*3].y = nply;
		}

		//If too small skip (due to errors)
		if(abs((setplanes[j].p[0].x + setplanes[j].p[0].y) - (setplanes[j].p[3].x + setplanes[j].p[3].y)) < 150)
			continue;

		/*TODO break up into plane per grid*/
		/*Lining up to grid*/
		for(int i = 0; i < 2; i++) {
			int mod = (int)(setplanes[j].p[i*3].x) % 400;

			if(mod > 200)
				setplanes[j].p[i*3].x = ((int)(setplanes[j].p[0].x*.0025 + 1)) * 400 + 1;// making sure it goes into the next grid
			else
				setplanes[j].p[i*3].x = (int)(setplanes[j].p[0].x*.0025) * 400 + 2;

			mod = (int)(setplanes[j].p[i*3].y) % 400;
			if(mod > 200)
				setplanes[j].p[i*3].y = ((int)(setplanes[j].p[0].y*.0025 + 1)) * 400 + 1;
			else
				setplanes[j].p[i*3].y = (int)(setplanes[j].p[0].y*.0025) * 400 + 2;

		}

		//Making sure no diagnol
		if(abs(setplanes[j].p[3].x - setplanes[j].p[0].x) > 10 && abs(setplanes[j].p[3].y - setplanes[j].p[0].y) > 10)
			continue;

		/*making sure higher is on bot*/
		if(setplanes[j].p[0].x - 10 > setplanes[j].p[3].x || setplanes[j].p[0].y - 10 > setplanes[j].p[3].y) {
			obj_point temp;
			temp = setplanes[j].p[0];
			setplanes[j].p[0] = setplanes[j].p[3];
			setplanes[j].p[3] = temp;
		}

		setplanes[j].p[1] = setplanes[j].p[0];
		setplanes[j].p[1].z = 0;
		setplanes[j].p[2] = setplanes[j].p[3];
		setplanes[j].p[3].z = 0;

		/*checking if plane exists*/
		bool match = false;
		for(int k = 0; k < plane_list.size(); k++) {

			if(abs(plane_list[k].p[0].x - setplanes[j].p[0].x) > 10 || abs(plane_list[k].p[0].y - setplanes[j].p[0].y) > 10)
				continue;

			match = true;
			if(plane_count[k] <= 100)
				plane_count[k] += 10;

			break;
		}
		if(!match) {
			plane_list.push_back(setplanes[j]);
			plane_count.push_back(10);
		}
	}
	if(setplanes.size() != 0)
		updategrid();
}


//Removing any planes that fail the test
void world_map::checkplanes() {
	/*if(abs(dir - 4.71) < .05 || abs(dir - 1.57) < .05) {
		// 1.57 = looking positive Y
		// 4.71 = negative y
	}
	else {
		//3.14 = positive x
		//0 = negative x
	}*/
}

void world_map::GetGrid(grid_space *set_grid) const {
	for(int i = 0; i < 121; i++) {
		set_grid[i].x = grid[i].x;
		set_grid[i].y = grid[i].y;
		set_grid[i].tags = grid[i].tags;

	}
}

void world_map::updategrid() {
	obj_point corners[2];

	for(std::vector<obj_plane>::const_iterator i = plane_list.begin(); i != plane_list.end(); i++){
		int st,en;
		int mag = 1;


		int x,y,x1,y1;
		x = i->p[0].x / 400;
		y = i->p[0].y / 400;
		x1 = i->p[3].x / 400;
		y1 = i->p[3].y / 400;

		if(x - x1 == 0)
			mag = width;

		st = x + (y * width);
		en = x1 + (y1 * width);

		//Flipping values if st is higher
		if(st > en) {
			int temp = st;
			st = en;
			en = temp;
		}

		while(st < en) {
			grid[st].tags |= non_traversable;
			st += mag;
		}
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
	updategrid();
	
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


