#ifndef NAI_MEMORY_WORLDMAP
#define NAI_MEMORY_WORLDMAP
/****************************************************************
	This objects will store and handle our collection of objects
	in the world.

	Maybe also pathfinding will be done in here returning a list of
	points to travel to?

****************************************************************/
#include "Infoproc/objects.hpp"
#include <math.h>

struct grid_space{
	int weight,parent,x,y;  //parent is the grid array iterator of the parent grid space
	uint8_t tags;
	int likelyness;
};


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


class world_map {
	public:
		world_map();
		~world_map();

		//std::vector<2d_obj> GetObjsinview(perspective data) //This will be an optimization feature to check for objects ahead of time
		const std::vector<obj_plane> &GetPlanes() const;
		const obj_cube *GetRobot() const;
		void GetGrid(grid_space *) const;
		void checkplanes();
		void updategrid();

		void addobjs(std::vector<obj_base>&);
		void gentest();
		void SetRobotAttr(obj_point,float);
		void AddPlanes(std::vector<obj_plane> &);

	private:
		obj_cube robot; //rot, 90 = positive down the x axis, 0 = positive down the y axis, 180 = negative down y axis, 270 = negative down x axis
		std::vector<obj_cube> entities_list;
		std::vector<obj_plane> plane_list;
		std::vector<int> plane_count; //how many times has this plane been successfully found;

		//grid data
		grid_space grid[121];
		int maptodate; //map version # to see if needs updating
		unsigned int width,size;
};

#endif
