#include "Memory/path_finding.h"


path_finding::path_finding(world_map *setmap) {
	wmap = setmap;
}

path_finding::~path_finding() {
	

}


//function returns the four neighbor points to point pt, but only those that are traversable and inside the grid
std::vector<int> path_finding::neighbors(int pt){
	std::vector<int> ret;//return vector

	int x = pt%width;
	int y = pt/width;

	if(!((x+1 > 11) && (grid[pt].tags | non_traversable)))//these ifs only add the neighbor if it is not outside the 11 x 11 grid
		ret.push_back(x + 1 + y*width);//and if it is traversable
	if(!(x-1 <= 0) && (grid[pt].tags | non_traversable))
		ret.push_back(x - 1 + y*width);
	if(!(y+1 > 11) && (grid[pt].tags | non_traversable))
		ret.push_back(x + (y+1)*width);
	if(!(y-1 <= 0) && (grid[pt].tags | non_traversable))
		ret.push_back(x + (y-1)*width);


	return ret;
}

int path_finding::get_dist(int st, int end){
	return (std::abs((end%width)-(st%width))+std::abs((end/width)-(st/width)));
}

std::vector<obj_point> path_finding::coarsepathfind(obj_point gl) {



	std::vector<int> open_set, closed_set, neighbors, path;



	neighbors = path_finding::neighbors(1);
	std::cout << "neighbors of 1 are: ";
	for (int i = 0; i < neighbors.size(); i++){
		std::cout << neighbors[i] << std::endl;
	}
	std::cout << std::endl;

	neighbors = path_finding::neighbors(60);
	std::cout << "neighbors of 60 are: ";
	for (int i = 0; i < neighbors.size(); i++){
		std::cout << neighbors[i] << std::endl;
	}
	std::cout << std::endl;





	obj_point rob_pos_obj = wmap->GetRobot().pos;
	std::vector<obj_point> ret_path;
	int current, goal, rob_pos=(((int)rob_pos_obj.x/400)+(((int)rob_pos_obj.y/400)*width));
			std::cout << rob_pos << std::endl;
	bool done = false;
	goal = (gl.x/400)+((gl.y/400)*width);

	std::cout << "goal is: " << goal << std::endl;
	std::cout << "distance from 1: " << path_finding::get_dist(1,goal) << std::endl;
	std::cout << "distance from 10: " << path_finding::get_dist(10,goal) << std::endl;
	std::cout << "distance from 121: " << path_finding::get_dist(121,goal) << std::endl;
	std::cout << "distance from 15: " << path_finding::get_dist(15,goal) << std::endl;
	std::cout << "distance from 60: " << path_finding::get_dist(60,goal) << std::endl;
	std::cout << std::endl;






	 // divide get_robot.pos().x and y by 400 and tag the square it's in as the starting square, save the number to so you can just use that
	grid[rob_pos].tags | is_robot;

	grid[rob_pos].weight = get_dist(rob_pos, goal); 

	open_set.push_back(rob_pos);
	int count = 0;
	int least_weighted = open_set[0];
	while(least_weighted != goal && count < 1000){												//main loop

		current = open_set.front();
		
		for(int i =1; i < open_set.size(); ++i){
			std::cout << "sort it" << std::endl;
			if (grid[current].weight >= grid[open_set[i]].weight)
				current = open_set[i];
		}//............................................................................................................................set the next grid space to be evaluated to the lowest weighted grid space in frontier

		closed_set.push_back(current);//...............................................................................................put that same grid space in the "has been evaluated" vector

		std::vector<int> neighbors_ = this->neighbors(current);//......................................................................generate the list of the neighboring spaces to the current grid space
		std::cout << "neighbors_ size is: " << neighbors_.size() << std::endl;//for debugging
		for(int i=0; i < neighbors_.size(); ++i){//....................................................................................for every neighbor in the list of neighbors:
			
			grid[neighbors_[i]].weight = get_dist(neighbors_[i], goal);//..........................................................set the weight to the manhattan distance to the goal grid space

			if(this->contains(open_set, neighbors_[i]) && (grid[neighbors_[i]].weight > grid[current].weight)){//..................if the current neighbor is in the "frontier" and is closer than the current:
				std::cout << "open set size 1 " << open_set.size() << " closed set size " << closed_set.size() << std::endl;
				open_set.erase(std::search_n(open_set.begin(), open_set.end(),1,neighbors_[i]));//.............................remove that neighbor from the "to be evaluated" frontier b/c current is better
			}
			else if((this->contains(closed_set, neighbors_[i])) && (grid[neighbors_[i]].weight < grid[current].weight)){//........if the current neighbor has been checked and is better than the current space:
				std::cout << "open set size 2 " << open_set.size() << " closed set size " << closed_set.size() << std::endl;
				open_set.erase(std::search_n(closed_set.begin(), closed_set.end(),1,neighbors_[i]));//.........................remove the neighbor from the closed set because that neighbor is closer.

			}
			else{//................................................................................................................if the neighbor has not been selected for evaluation yet:
				std::cout << "open set size 3 " << open_set.size() << " closed set size " << closed_set.size() << std::endl;
				open_set.push_back(neighbors_[i]);
				for(int i = 0; i < open_set.size() ; i++){//prints weights
					std::cout << open_set[i] << "'s weight is: " << grid[open_set[i]].weight<< std::endl;
				}
				std::cout << "\n\n\n\n\n\n" << std::endl;
				grid[neighbors_[i]].parent = current;//........................................................................set the new frontier space's parent to the space it came from
			}

		}
		for(int i =1; i < open_set.size(); ++i){
			std::cout << "sort it" << std::endl;
			if (grid[current].weight >= grid[open_set[i]].weight)
				least_weighted = open_set[i];
		}
		std::cout << "end it" << std::endl;
		count++;
	}//....................................................................................................................................end main loop
	path.push_back(goal);
	goal = current;
	count = 0;
	while(current != rob_pos && count < 121){
		std::cout << "nooo" << std::endl;
		path.push_back(grid[current].parent);
		current = grid[current].parent;
		count++;
	}
	std::reverse(path.begin(),path.end());
	std::cout << "current path is: ";
	for(int i = 0; i < path.size() ; i++){
		std::cout << path[i] << ' ';
	}
	std::cout<<std::endl;
	obj_point obj;

	//TODO: convert path to obj_point
	for(int i = 0; i < path.size(); i++){
		obj.x = path[i]%width * 400 + 200;
		obj.y = path[i]/width * 400 + 200;
		ret_path.push_back(obj);
	}	

	return ret_path;

}

bool path_finding::generate_grid(void){
	std::vector<obj_plane> planes = wmap->GetPlanes();
	obj_point corners[4];

	for( int i = 0; i< planes.size(); ++i){
		//corners = {0,0,0,0};
		for(int j=0;j < 4; j++){
			//corners[j] = planes[i].p[j]/400;//scale down current plane's corners into grid sized corners
		}
		
		
	}
	return true;
}

bool path_finding::contains(std::vector<int> search_vect, int tar){
	for(int i = 0; i < search_vect.size(); ++i){
		if(search_vect[i] == tar)
			return true;
	}
	return false;
}

std::vector<obj_point> path_finding::gotopoint(obj_point findpoint) {
	std::vector<obj_point> returnpoint;

	obj_point newp;
	newp.z = 50.0f;
	//Don't touch above
	
	returnpoint = coarsepathfind(findpoint);

	for(int i = 0; i < returnpoint.size(); ++i){
		std::cout << returnpoint[i].x << ' ' << returnpoint[i].y << std::endl;
	}

	/*//Testpoint
	newp.x = 200.0f;
	newp.y = 2200.0f;
	returnpoint.push_back(newp);
	*/

	//Don't touch below
	return returnpoint;
}

