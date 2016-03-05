#include "Infoproc/path_finding.h"


path_finding::path_finding(const world_map *set_map) : wmap(set_map), robot(set_map->GetRobot()) {
	pathid = 1;
	pathgood = true;
	wmap->InitGrid(grid);
}

path_finding::~path_finding() {
}


void path_finding::checkpath(){
	if(pathgood == true) {
		const grid_space *gridhold = wmap->GetGrid();
		for(int i = 0; i < curpath.size(); i++) {
			unsigned int pos = (curpath[i].x*.0025) + (curpath[i].y*.0025)*width;

			if((gridhold[i].tags & non_traversable) && gridhold[i].likelyness < 10)
					pathgood = false;		
		}
	}

	if(!pathgood) {
		obj_point resetgoal;
		resetgoal.x = goal % width;
		resetgoal.y = goal / width;
		coarsepathfind(resetgoal);
	}
}


//function returns the four neighbor points to point pt, but only those that are traversable and inside the grid
void path_finding::getneighbors(int pt, std::vector<int> &ret){
	int x = grid[pt].x;
	int y = grid[pt].y;
	

	if(!(x+1 >= 11 || grid[pt].tags & non_traversable))//these ifs only add the neighbor if it is not outside the 11 x 11 grid
		ret.push_back(x + 1 + y*width);//and if it is traversable
	if(!(x-1 < 0 || grid[pt].tags & non_traversable)) 
		ret.push_back((x - 1) + y*width);
	if(!(y+1 >= 11 || grid[pt].tags & non_traversable))
		ret.push_back(x + (y+1)*width);
	if(!(y-1 < 0 || grid[pt].tags & non_traversable))
		ret.push_back(x + (y-1)*width);
}

int path_finding::get_dist(int st, int end){
	return (abs((grid[end].x)-(grid[st].x))+abs((grid[end].y)-(grid[st].y)));
}


bool path_finding::coarsepathfind(obj_point gl) {
	int current;
	goal = generate_grid(gl);
	std::stack<int> open_set;
	obj_point rob_pos_obj = robot->pos;

	 // divide get_robot.pos().x and y by 400 and tag the square it's in as the starting square, save the number to so you can just use that
	int rob_pos=((rob_pos_obj.x/400)+((int)(rob_pos_obj.y/400)*width));
	grid[rob_pos].tags |= (is_robot | gopened);
	open_set.push(rob_pos);

	while(open_set.top() != goal){ //main loop
		current = open_set.top();
		std::vector<int> neighbors_;

		getneighbors(current, neighbors_);
		grid[open_set.top()].tags |= gopened ;
		open_set.pop();

		//for every neighbor in the list of neighbors:
		for(int i=0; i < neighbors_.size(); ++i){
			//TODO Check its length (length being how many steps away to see if going this way is faster)
			if((grid[neighbors_[i]].tags & gopened)) 
				continue;

			grid[neighbors_[i]].parent = current;

			//Adding neighbor and organizing by weight
			std::stack<int> temp;
			while(true){ //While(true) to keep everthing inside this loop
				grid[neighbors_[i]].tags |= gopened;
				if(open_set.empty() || grid[open_set.top()].weight > grid[neighbors_[i]].weight) {
					open_set.push(neighbors_[i]);
					while(!temp.empty()) {
						open_set.push(temp.top());
						temp.pop();
					}
					break;
				}
				temp.push(open_set.top());
				open_set.pop();
			}

		}
		if(open_set.empty())
			return false;
	}
	current = goal;

	obj_point obj;
	obj.z = 50;

	curpath.clear();
	//convert path to obj_point
	while(current != -1){
		obj.x = grid[current].x * 400 + 200;
		obj.y = grid[current].y * 400 + 200;
		curpath.push_back(obj);
		current = grid[current].parent;
	}

	return true;
}

int path_finding::generate_grid(obj_point &gl){
	const grid_space *gridhold = wmap->GetGrid();

	for(int i = 0; i < 121; i++) {
		grid[i].tags = gridhold[i].tags;
		if((gridhold[i].tags & non_traversable) && gridhold[i].likelyness < 10 || gridhold[i].likelyness == -2)
			grid[i].tags |= ~non_traversable;
	}

	int goal = (gl.x*0.0025) + ((int)(gl.y*0.0025) * 11);

	for(int i = 0; i < 121; ++i) {
		grid[i].weight = get_dist(i, goal);
		grid[i].parent = -1;
	}

	return goal;
}


int path_finding::xy2griterator(int x, int y){
	return x+(y*width);
}

bool path_finding::contains(std::vector<int> search_vect, int tar){
	for(int i = 0; i < search_vect.size(); ++i)
		if(search_vect[i] == tar)
			return true;

	return false;
}

bool path_finding::doorcheck() const {
	obj_point nextgrid = robot->pos;

	if(robot->rot < .09 )
		robot->pos.x + 400;
	else if (abs(robot->rot - 3.14) < .09)
		robot->pos.x - 400;
	else if (abs(robot->rot - 1.57) < .09)
		robot->pos.y + 400;
	else if (abs(robot->rot - 4.71) < .09 )
		robot->pos.y - 400;
	else return false;

	int gridspace = (nextgrid.x / 400) + ((nextgrid.y / 400)*11);
	if(gridspace < 0 && gridspace > 120)
		return false;

	return (grid[gridspace].tags & door);

}

bool path_finding::gotopoint(obj_point findpoint) {
	if(!coarsepathfind(findpoint))
		return false;

	pathgood = true;
	pathid++;
	return true;
}

unsigned int path_finding::GetPathID() const {
	return pathid;
}
const std::vector<obj_point> &path_finding::GetPath() const {
	return curpath;

}

