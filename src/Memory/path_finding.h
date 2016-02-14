#ifndef NAI_PATH_FINDING
#define NAI_PATH_FINDING
#include "Memory/world_map.h"
#include <vector>
#include <algorithm>
#include <iostream> 
#include <math.h>
#include <stack>

enum node_flag{
    non_traversable=0x01,
    dispenser=0x02,
    cannon=0x04,
    goal=0x08,
    gballbox=0x10,
    my_gballbox=0x20,
    is_robot=0x40,
	gopened=0x80,
	none=0x00};

inline node_flag operator|(node_flag a, node_flag b)
{return static_cast<node_flag>(static_cast<int>(a) | static_cast<int>(b));}

struct grid_space{
	int weight,parent,x,y;  //parent is the grid array iterator of the parent grid space
	uint8_t tags;
};


class path_finding {
	public:
		path_finding(const world_map*);
		~path_finding();

		bool findpathtoobj(std::string);
		bool searchat(obj_point);
		std::vector<obj_point> gotopoint(obj_point);
		
		

	private:
		//Coarse pathfinding
		inline bool coarsepathfind(obj_point, std::vector<obj_point>&);
		inline void getneighbors(int, std::vector<int>&);
		inline int get_dist(int, int); //returns manhattan distance between st and end
		inline bool contains(std::vector<int>, int);
		inline int xy2griterator(int, int);
		inline int generate_grid(obj_point&);

		grid_space grid[121];
		int width = 11;
		const world_map *wmap;

	};
#endif
