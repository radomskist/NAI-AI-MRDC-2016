#include "Infoproc/world_map.h"

#include <iostream>
world_map::world_map() : robot("NAI") {
	robot.pos.x = 200;
	robot.pos.y = 200;
	robot.pos.z = 50;

	mapversion = 0;
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
		grid[i].likelyness = 0;
	}
}

unsigned int world_map::GetMapVersion() const {
	return mapversion;
}

//TODO check seeing through walls
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
		//if((abs(setplanes[j].p[0].x - setplanes[j].p[3].x) + abs(setplanes[j].p[0].y - setplanes[j].p[3].y)) < 10)
		//	continue;

		/*Lining up to grid*/
		obj_plane tempplane(1,1);
		for(int i = 0; i < 2; i++) {
			int mod = (int)(setplanes[j].p[i*3].x) % 400;

			//This is rounding to the nearest grid space depending on the angle that we're looking at

			//setting up direction mod for x axis
			int dirmody = 0;
			int dirmodx = 0;
			if(robot.rot < .1 || abs(robot.rot - 6.14) < .1) {
				dirmodx = 1;
				dirmody = 0;
			}
			else if(abs(robot.rot - 3.14) < .1) {
				dirmodx = 0;
				dirmody = -1;
			}
			//Setting up for Y axis
			else if(abs(robot.rot - 1.57) < .1) {
				dirmodx = 0;
				dirmody = 1;
			}
			else if(abs(robot.rot - 4.71) < .1) {
				dirmodx = -1;
				dirmody = 0;
			}

			//rounding x
			if(mod > 200)
				tempplane.p[i*3].x = (int)(setplanes[j].p[i*3].x*.0025 + dirmodx);// making sure it goes into the next grid
			else
				tempplane.p[i*3].x = (int)(setplanes[j].p[i*3].x*.0025 );


			mod = (int)(setplanes[j].p[i*3].y) % 400;
			//rounding y
			if(mod > 200) 
				tempplane.p[i*3].y = (int)(setplanes[j].p[i*3].y*.0025 + dirmody);// making sure it goes into the next grid
			else
				tempplane.p[i*3].y = (int)(setplanes[j].p[i*3].y*.0025);// making sure it goes into the next grid

		}

		//Making sure no diagnol
		if(abs(tempplane.p[3].x - tempplane.p[0].x) > 1 && abs(tempplane.p[3].y - tempplane.p[0].y) > 1)
			continue;

		/*making sure higher is on bot*/
		if(abs(tempplane.p[0].x - 1) > tempplane.p[3].x || abs(tempplane.p[0].y - 1) > tempplane.p[3].y) {
			obj_point temp;
			temp = tempplane.p[0];
			tempplane.p[0] = tempplane.p[3];
			tempplane.p[3] = temp;
		}

		tempplane.p[1] = tempplane.p[0];
		tempplane.p[1].z = 0;
		tempplane.p[2] = tempplane.p[3];
		tempplane.p[3].z = 0;

		/*checking if plane exists*/
		//if X
		if(abs(tempplane.p[0].x - tempplane.p[3].x) > 1) {
			int start = tempplane.p[0].x + tempplane.p[0].y * width;
			int end = tempplane.p[3].x - tempplane.p[0].x;
			for(int k = 0; k <= end; k++) {
				int gridspot = start + k;
				if(gridspot > 121 || gridspot < 0)
					break;

				if(grid[gridspot].tags & non_traversable) 
					grid[gridspot].likelyness += 5;
				else {
					obj_wall tempwall;
					tempwall.pos.x = (tempplane.p[0].x + k);
					tempwall.pos.y = (tempplane.p[0].y);
					tempwall.pos.z = 150;
					tempwall.width = 200;
					tempwall.height = 300;
					tempwall.draw = false;

					grid[gridspot].likelyness = 10;
					plane_list.push_back(tempwall);
				}
			}
		}
		else {
			int start = tempplane.p[0].x + tempplane.p[0].y * width;
			int end = tempplane.p[3].y - tempplane.p[0].y;
			for(int k = 0; k <= end; k++) {
				int gridspot = start + width*k;
				if(gridspot > 121 || gridspot < 0) 
					break;
				
				if(grid[gridspot].tags & non_traversable) 
					grid[gridspot].likelyness += 5;
				else {
					obj_wall tempwall;
					tempwall.pos.x = tempplane.p[0].x;
					tempwall.pos.y = (tempplane.p[0].y + k);
					tempwall.pos.z = 150;
					tempwall.width = 200;
					tempwall.height = 300;
					tempwall.draw = false;

					grid[gridspot].likelyness = 10;
					plane_list.push_back(tempwall);
				}
			}
		}

	}
	if(setplanes.size() != 0)
		updategrid();
}


//Removing any planes that fail the test
void world_map::checkplanes(int pointvalues[5]) {
	//for(int i = 0; i < 5; i++)
	//	std::cout << pointvalues[i] << std::endl;

	//If fails within one gridspace than assume we're misaligned
	//TODO: this can be error prone, needs heavy testing
	int dirx = 0;
	int diry = 0;
	int robx = robot.pos.x *.0025;
	int roby = robot.pos.y * .0025;
	int basepos = robx + roby*width;

	if(abs(robot.rot - 4.71) < .1)
		diry = -1;
	else if (abs(robot.rot - 1.57) < .1) 
		diry = 1;
	else if (abs(robot.rot - 3.14) < .1) 
		dirx = -1;
	else if (robot.rot < .05) 
		dirx = 1;
	else return;

	bool edgehit = false;
	int dist = 0;
	int pos = basepos;
	int distleft = 0;
	int distright = 0;
	while(!edgehit) {
		if(grid[pos].tags & non_traversable) {
			break;
		}

		if(dirx < 0 && basepos + robx*dirx > 0)
			dirx -= 1;
		else if (dirx > 0 && dirx < (width - 2)) 
			dirx += 1;
		else if (diry > 0 && diry < (width - 2))
			diry += 1;
		else if(diry < 0 && basepos + roby*diry*width > 0) 
			diry -= 1;
		else
			return;

		pos = basepos + dirx*robx + (diry*roby* width);
		dist = abs(grid[pos].x - robx) + abs(grid[pos].y - roby);
	}

	dist *= 400;

	//too far away
	if(pointvalues[2] - dist > 200)
		grid[pos].likelyness -= 2;
	//Within range
	else if(abs(pointvalues[2] - dist) > 200) {
		grid[pos].likelyness += 2;
		return;
	}
	//Too close
	//TODO: see if we're not moving
	else {
		
		
	}


}


//TODO CHECK FOR BUGS
//Might be causing there to be invisible non-traversable flooring
void world_map::InitGrid(grid_space *set_grid) const {
	for(int i = 0; i < 121; i++) {
		set_grid[i].x = grid[i].x;
		set_grid[i].y = grid[i].y;
	}
}

const grid_space *world_map::GetGrid() const {
	return grid;
}

void world_map::updategrid() {
	mapversion++;
	for(std::vector<obj_wall>::iterator i = plane_list.begin(); i != plane_list.end(); i++) {
		unsigned int spot = (i->pos.x + (i->pos.y)*width);
		grid[spot].tags |= non_traversable;
		if(grid[spot].likelyness > 15)
			i->draw = true;

		//Removing walls that fail
		else if (grid[spot].likelyness < 10) {
			plane_list.erase(i);
			grid[spot].tags |= ~non_traversable;
		}
	}

}


world_map::~world_map() {
	
}

void world_map::gentest() {
	//one unit = 100th of a foot
	//so 400 units is 4 feet

	//Creating a new plane	
	obj_wall newplane = obj_wall();
	newplane.pos.y = 1;
	newplane.width = 200;
	newplane.pos.z = 150;
	newplane.height = 150;
	for(int i = 1; i < 6; i++) {
		newplane.pos.x = i;
		unsigned int spot = (newplane.pos.x + (newplane.pos.y)*width);
		grid[spot].likelyness = 500;
		plane_list.push_back(newplane);
	}

	newplane.pos.x = 1;
	for(int i = 0; i < 2; i++) {
		newplane.pos.y = i;
		unsigned int spot = (newplane.pos.x + (newplane.pos.y)*width);
		grid[spot].likelyness = 500;
		plane_list.push_back(newplane);
	}

	updategrid();
	
}

void world_map::SetRobotAttr(obj_point set_pos, float set_ang) {
	robot.pos = set_pos;
	robot.rot = set_ang;
}

const obj_cube *world_map::GetRobot() const {
	return &robot;
}

const std::vector<obj_wall> &world_map::GetPlanes() const {
	return plane_list;
}

void world_map::addobjs(std::vector<obj_base> &add_obj) {
	
}


