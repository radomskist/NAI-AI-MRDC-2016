#ifndef NAI_STATE_READ_QR
#define NAI_STATE_READ_QR
#include "Infoproc/states/base_state.h"

class read_qr : public base_state {

	public:
		read_qr(const world_map*);
		~read_qr();

		int Process();

	protected:

};

#endif
