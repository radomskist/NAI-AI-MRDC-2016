#ifndef NAI_STATE_FIRST_DOOR
#define NAI_STATE_FIRST_DOOR
#include "Infoproc/world_map.h"
#include "Infoproc/states/base_state.h"
#include "Infoproc/path_finding.h"
#include "Infoproc/states/go_inside.h"
class first_door : public base_state {

	public:
		first_door(const world_map*, path_finding&);
		~first_door();

		void intialize();
		int Process();
		void SetStat(std::string);
		base_state *endstate();
	protected:
		bool scanning;
		bool scanattempt;
		bool readytoopen;
		bool qrread;
		bool readingqr;
		bool turning;
		path_finding &pfind;
};

#endif
