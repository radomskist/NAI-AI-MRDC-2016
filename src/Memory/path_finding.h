#ifndef NAI_PATH_FINDING
#define NAI_PATH_FINDING
#include "Memory/world_map.h"

class path_finding {
	public:
		path_finding(world_map*);
		~path_finding();

		bool findpathtoobj(std::string);
		bool searchat(obj_point);
		std::vector<obj_point> gotopoint(obj_point);


	private:
		//Coard pathfinding
		std::vector<obj_point> coarsepathfind(obj_point);
		float grid[11][11];

		world_map *wmap;

	};
#endif
