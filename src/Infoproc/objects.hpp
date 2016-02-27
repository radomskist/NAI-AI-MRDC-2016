#ifndef _NAIBRAIN_OBJBASE
#define _NAIBRAIN_OBJBASE

#include <string>
#include <vector>
enum obj_type {UNDEF = 0, POINT = 1, PLANE = 4, CUBE = 8, WALL = 64};

struct obj_point {
	public:
		obj_point() {
			x = 0;
			y = 0;
			z = 0;
		}

		obj_point(const obj_point &set) {
			x = set.x;
			y = set.y;
			z = set.z;
		}

		obj_point& operator=(const obj_point& set) {
			x = set.x;
			y = set.y;
			z = set.z;
		}


		obj_point(float sx,float sy, float sz) {
			x = sx;
			y = sy;
			z = sz;
		}

		float x,y,z;
};

/****************************************************************
   FUNCTION:	This is the base object for objects we will store in memory
				Planes, points, maybe even cubes.

   NOTES:		This is a layout for a future implementation and can be
				subject to change.

****************************************************************/

//This class might or might not be abstract

//Planned to be the base object for data we get
struct obj_base {
	public:
		obj_base() {
			type = UNDEF;
			}

		int GetType() {
			return type;
		}

	protected:
		int type; //Type we're dealing with
		/*Also used for how many points Regular objects have 1, planes have 4, cubes have 8 (cubes might be for things like other robots and the space they occupy)*/
	};

/****************************************************************
   FUNCTION:	This is the layout for plane objects

   NOTES:		This is a layout for a future implementation and can be
				subject to change.

****************************************************************/
struct obj_plane : public obj_base {
	public:
		obj_plane(float sslopex, float sslopey) {
			slopex = sslopex;
			slopey = sslopey;
			type = PLANE;
		}

		float slopex, slopey; //Slope will be important for quick comparisons!
		obj_point p[4];


};

/****************************************************************
   FUNCTION:	This is the cube object which will identify objects
				in the world. (balls and such)

   NOTES:		This is a layout for a future implementation and can be
				subject to change.

****************************************************************/

struct obj_wall : public obj_base {
	public:
		obj_wall() {
			type = WALL;
		}

		obj_point pos;
		float width, height;
	private:
		int type; //Type we're dealing with
		/*Also used for how many points Regular objects have 1, planes have 4, cubes have 8 (cubes might be for things like other robots and the space they occupy)*/
};

struct obj_cube : public obj_base {
	public:
		obj_cube(std::string set_name) {
			name = set_name;
			type = CUBE;
		}

		std::string GetName() {
			return name;
		}

		std::vector<std::string> tags; //dataname and tag
		obj_point pos;
		float width, height,rot;
		float color[3];
		
	private:
		std::string name;
		int type; //Type we're dealing with
		/*Also used for how many points Regular objects have 1, planes have 4, cubes have 8 (cubes might be for things like other robots and the space they occupy)*/
};


#endif
