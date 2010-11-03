#include "libs.h"

class SignalHander
{
	private:
		sigset_t setBlocked;
		sigset_t setHandled;
	
	public:
		static void handler( int sigNum );
		int setSigMasked (bool masked );
};
