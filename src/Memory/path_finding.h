#ifndef NAI_PATH_FINDING
#define NAI_PATH_FINDING
#include "Memory/world_map.h"
#include <vector>
#include <algorithm>
#include <iostream> 
#include <math.h>

enum node_flag{
    non_traversable=0x01,
    dispenser=0x02,
    cannon=0x04,
    goal=0x08,
    gballbox=0x10,
    my_gballbox=0x20,
    is_robot=0x40};

inline node_flag operator|(node_flag a, node_flag b)
{return static_cast<node_flag>(static_cast<int>(a) | static_cast<int>(b));}

struct grid_space{
	unsigned long weight,parent;  //parent is the grid array iterator of the parent grid space
	uint8_t tags;
};


class path_finding {
	public:
		path_finding(const world_map&);
		~path_finding();

		bool findpathtoobj(std::string);
		bool searchat(obj_point);
		std::vector<obj_point> gotopoint(obj_point) const;
		
		

	private:
		//Coarse pathfinding
		std::vector<obj_point> coarsepathfind(obj_point);
		std::vector<int> neighbors(int);
		int get_dist(int, int); //returns manhattan distance between st and end
		bool contains(std::vector<int>, int);
		int xy2griterator(int, int);
		grid_space grid[121];
		int width = 11;
		void generate_grid(void);
		const world_map &wmap;

	};
#endif
