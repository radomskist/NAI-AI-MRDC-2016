#include "Infoproc/states/read_qr.h"

read_qr::read_qr(world_map &set_map, kinectman& set_kinect) : nonconstwmap(set_map), kinect_manager(set_kinect) {

}

read_qr::~read_qr() {

}

int read_qr::Process() {

	std::cout << "QR SCNANIN" << std::endl;
	//Request to scan image see how far off from the center we are
	float distz = 0; //dist from bot
	int distfc = 0; //distance from center
	std::string code; //qr code

	distz = kinect_manager.qrscan(&distfc, &code);

	if(abs(distfc) > 40) {
		//Strafe to line up with image
	}
	else {
		//DOOR CODE
		if(code[0] == 'D') {
			obj_point set_pos;
			//might be flipped
			set_pos.x = std::stoi(code.substr(1,2));
			set_pos.y = std::stoi(code.substr(3,4));
			set_pos.z = 50;
			nonconstwmap.SetRobotAttr(set_pos,0);

			//Adding test door
			obj_cube testdoor("door");
			testdoor.rot = nonconstwmap.GetRobot()->rot - 3.14;
			if(testdoor.rot < 0)
				testdoor.rot + 6.14;
			else if(testdoor.rot > 6.14)
				testdoor.rot - 6.14;

			testdoor.color[0] = .2;
			testdoor.color[1] = .5;
			testdoor.color[2] = 1.0;
			testdoor.height = 300;
			testdoor.width = 400;
			testdoor.pos.z = 150;
			testdoor.pos.x = set_pos.x ;
			testdoor.pos.y = set_pos.y;
			std::vector<obj_cube> list;
			list.push_back(testdoor);

			/*wall spots*/
			int xmod = 0;
			int ymod = 0;
			if(abs(1.57 - testdoor.rot) < .05 || abs(4.71 - testdoor.rot) < .05)
				ymod = 400;
			else
				xmod = 400;

			obj_cube walls("wall");
			walls.pos.z = 150;
			walls.pos.x = set_pos.x + xmod;
			walls.pos.y = set_pos.y + ymod;
			list.push_back(walls);

			walls.pos.x = set_pos.x - xmod;
			walls.pos.y = set_pos.y - ymod;
			list.push_back(walls);

			if(abs(1.57 - testdoor.rot) < .05 || abs(4.71 - testdoor.rot) < .05) {
				xmod = 400;
				ymod = -400;
			}
			else {
				ymod = 400;
				xmod = -400;
			}

			obj_cube blanks("blank");
			blanks.pos.z = 150;
			for(int i = 0; i < 3; i++) {
				if(xmod < 0) {
					walls.pos.x = set_pos.x + (xmod*i);
					list.push_back(walls);

					blanks.pos.x = walls.pos.x;
					blanks.pos.y = set_pos.y + ymod;
					list.push_back(blanks);

					blanks.pos.y = set_pos.y - ymod;
					list.push_back(blanks);
				}
				else {
					walls.pos.y = set_pos.y + (xmod*i);
					list.push_back(walls);

					blanks.pos.y = walls.pos.y;
					blanks.pos.x = set_pos.x + xmod;
					list.push_back(blanks);

					blanks.pos.x = set_pos.x - xmod;
					list.push_back(blanks);		
				}

			}

			nonconstwmap.addobjs(list);
		}
		else if(code[0] == 'S') {

		}
		else if(code[0] == 'B') {

		}
	}

}
