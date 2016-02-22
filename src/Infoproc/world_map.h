#ifndef NAI_MEMORY_WORLDMAP
#define NAI_MEMORY_WORLDMAP
/****************************************************************
	This objects will store and handle our collection of objects
	in the world.

	Maybe also pathfinding will be done in here returning a list of
	points to travel to?

****************************************************************/
#include "Infoproc/objects.hpp"

class world_map {
	public:
		world_map();
		~world_map();

		//std::vector<2d_obj> GetObjsinview(perspective data) //This will be an optimization feature to check for objects ahead of time
		const std::vector<obj_plane> &GetPlanes() const;
		const obj_cube *GetRobot() const;

		void addobjs(std::vector<obj_base>&);
		void gentest();
		void SetRobotAttr(obj_point,float);

	private:
		obj_cube robot; //rot, 90 = positive down the x axis, 0 = positive down the y axis, 180 = negative down y axis, 270 = negative down x axis
		std::vector<obj_cube> entities_list;
		std::vector<obj_plane> plane_list;

		bool maptodate;

};

#endif
