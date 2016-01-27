/****************************************************************
	This objects will store and handle our collection of objects
	in the world.

	Maybe also pathfinding will be done in here returning a list of
	points to travel to?

****************************************************************/
#include "Memory/objects.hpp"

class world_map {
	public:
		world_map();
		~world_map();

		//std::vector<2d_obj> GetObjsinview(perspective data) //This will be an optimization feature to check for objects ahead of time
		void addobjs(std::vector<obj_base>&);
		bool findpathtoobj(std::string);
		bool searchat(obj_point);

	private:
		std::vector<obj_cube> entities_list;
		std::vector<obj_plane> plane_list;

};