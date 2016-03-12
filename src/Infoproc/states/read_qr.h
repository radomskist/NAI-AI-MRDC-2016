#ifndef NAI_STATE_READ_QR
#define NAI_STATE_READ_QR
#include "Infoproc/states/base_state.h"
#include "Devices/kinect_man.h"

class read_qr : public base_state {

	public:
		read_qr(world_map&, kinectman&);
		~read_qr();

		int Process();

	protected:
		kinectman &kinect_manager;
		world_map& nonconstwmap;
};

#endif
