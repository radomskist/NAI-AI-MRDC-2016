#ifndef _NAIBRAIN_OBJBASE
#define _NAIBRAIN_OBJBASE
/****************************************************************
   AUTHOR:		Steven Radomski
   LOGON ID:	Z1783380 
   DATE:		December, 29 2015

   FUNCTION:	This is the base object for objects we will store in memory
				Planes, points, maybe even cubes.

   NOTES:		This is a layout for a future implementation and can be
				subject to change.

****************************************************************/

//This class might or might not be abstract

//Planned to be the base object for data we get
#include <string>
#include <map>
enum obj_type {UNDEF = 0, POINT = 1, PLANE = 4, CUBE = 8};

class obj_base {
	public:
		obj_base(int set_type, float **set_points) {

			}

		~obj_base() { }

		std::map<std::string, std::string> &GetData() { //Returns point length, and point values
			return data;
			}; 

		int GetType() {return type;}

	private:
		std::map<std::string, std::string> data; //dataname and tag
		int type; //Type we're dealing with
		/*Also used for how many points Regular objects have 1, planes have 4, cubes have 8 (cubes might be for things like other robots and the space they occupy)*/
	};

#endif
