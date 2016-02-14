#include "Memory/path_finding.h"


path_finding::path_finding(const world_map &set_map) : wmap(set_map) {
	int size = width*width;
	for(int i = 0; i < size; i++) {
		grid[i].x = i % width;
		grid[i].y = i / width;
	}
}

path_finding::~path_finding() {
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


bool path_finding::coarsepathfind(obj_point gl, std::vector<obj_point> &ret_path) {
	int current;
	int goal = generate_grid(gl);

	std::stack<int> open_set;

	obj_point rob_pos_obj = wmap.GetRobot().pos;

	 // divide get_robot.pos().x and y by 400 and tag the square it's in as the starting square, save the number to so you can just use that
	int rob_pos=((rob_pos_obj.x/400)+((int)(rob_pos_obj.y/400)*width));
	grid[rob_pos].tags |= (is_robot | gopened);
	open_set.push(rob_pos);

	while(open_set.top() != goal){ //main loop
		current = open_set.top();
		std::vector<int> neighbors_;

		getneighbors(current, neighbors_);
		grid[open_set.top()].tags |= (~gopened | gclosed);
		open_set.pop();

		//for every neighbor in the list of neighbors:
		for(int i=0; i < neighbors_.size(); ++i){
			//TODO Check its length (length being how many steps away to see if going this way is faster)
			if((grid[neighbors_[i]].tags & (gclosed | gopened)) == (gclosed | gopened)) 
				continue;

			grid[neighbors_[i]].parent = current;

			//Adding neighbor and organizing by weight
			std::stack<int> temp;
			while(true){
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
	//convert path to obj_point
	while(current != -1){
		//std::cout << obj.x << "," << obj.y << std::endl;
		obj.x = current%width * 400 + 200;
		obj.y = current/width * 400 + 200;
		ret_path.push_back(obj);
		current = grid[current].parent;
	}

	return true;
}

int path_finding::generate_grid(obj_point &gl){
	std::vector<obj_plane> planes = wmap.GetPlanes();
	bool is_vert;
	obj_point corners[2];
	int dist;
	int goal = (gl.x*0.0025) + (gl.y*0.0025 * 11);

	std::string test_string;
	for(int i = 0; i < 121; ++i) {
		grid[i].weight = get_dist(i, goal);
		grid[i].parent = -1;
		grid[i].tags = none; //emptying flags

		/*if(grid[i].tags | non_traversable)
			test_string += "X";
		else
			test_string += "0";*/
	}

	for( int i = 0; i < planes.size(); ++i){

		for(int j=0; j<= 2; ++j){
			corners[j] = planes[i].p[j*2];//scale down current plane's corners into grid sized corners
			corners[j].x = std::nearbyint(corners[j].x * 0.0025);
			corners[j].y = std::nearbyint(corners[j].y * 0.0025);
		}

		corners[1].x - corners[0].x == 0 ? is_vert = true : is_vert = false;

		if(is_vert)
			for(int j = corners[0].y ; j <= corners[1].y; ++j)
				grid[xy2griterator(corners[0].x, corners[0].y+j)].tags | non_traversable;
		else
			for(int j = corners[0].x ; j <= corners[1].x; ++j)
				grid[xy2griterator(corners[0].x+j, corners[0].y)].tags | non_traversable;
	
	}


/*
	for(int i =1; i<= 10; ++i)
		for(int j = 0; i < 10; ++j)
			std::cout << test_string[j*i] << std::endl;*/
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

std::vector<obj_point> path_finding::gotopoint(obj_point findpoint) {

	std::vector<obj_point> returnpoint;
	if(!coarsepathfind(findpoint,returnpoint))
		std::cout << "Path failed" << std::endl;

	return returnpoint;
}

