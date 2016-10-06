/*****************************************************************
	* Author: Steven Radomski radomskist@yahoo.com
	*
	* Copyright (C) 2017 Steven Radomski
	* 
	* This file was part of the 2016 NIU robotics AI robot project
	* 
	* This code cannot be copied or used without the permission of
	* the author
	*
	*
*****************************************************************/
#ifndef NAI_PATH_FINDING
#define NAI_PATH_FINDING
#include "Infoproc/world_map.h"
#include <vector>
#include <algorithm>
#include <iostream> 
#include <math.h>
#include <stack>

class path_finding {
	public:
		path_finding(const world_map*);
		~path_finding();

		void checkpath();
		bool findpathtoobj(std::string);
		bool searchat(obj_point);
		bool gotopoint(obj_point);
		bool pathstatus();
		const std::vector<obj_point> &GetPath() const;
		unsigned int GetPathID() const;
		int doorcheck() const;

	private:
		//Coarse pathfinding
		inline bool coarsepathfind(obj_point);
		inline void getneighbors(int, std::vector<int>&);
		inline int get_dist(int, int); //returns manhattan distance between st and end
		inline bool contains(std::vector<int>, int);
		inline int xy2griterator(int, int);
		inline int generate_grid(obj_point&);
	
		int goal;
		int width = 11;
		const world_map *wmap;
		const obj_cube *robot;
		unsigned int pathid;
		grid_space grid[121];
		bool pathgood;
	
		std::vector<obj_point> curpath;

	};
#endif
