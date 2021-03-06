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
#ifndef NAI_STATE_ARG_BASE
#define NAI_STATE_ARG_BASE
#include "Infoproc/world_map.h"
#include "Infoproc/states/base_state.h"
#include <vector>
#include <array>
//Base class for states that take arguments
//Should only be 
class arg_state : public base_state {

	public:
		arg_state(const world_map* set_map, std::string set_args) : base_state(set_map) {
			
			//Parsing the string
			for(int i = 0; i < set_args.size(); i++) {

				int j = i;
				for(; j < set_args.size(); j++)
					if(set_args[j] == ':')
						break;

				std::string argcom = set_args.substr(i,(j-i));
				j++;
				i = j;
				for(; j < set_args.size(); j++)
					if(set_args[j] == ';')
						break;

				std::string value = set_args.substr(i,(j-i));
				i = j;
				std::array<std::string,2> newarg;
				newarg[0] = argcom;
				newarg[1] = value;
				args.push_back(newarg);
			}
		}

		~arg_state() {}

		const std::vector<std::array<std::string,2>>& GetArgs() const {
			return args;
		}

		virtual int Process() = 0; //Process information

	private:
		std::vector<std::array<std::string,2>> args;
};

#endif
