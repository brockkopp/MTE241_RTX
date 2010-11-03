#include "libs.h"
#include "signal.h"

class SignalHandler
{
	private:
		sigset_t setBlocked;
		sigset_t setHandled;
	
	public:
		SignalHandler();
		static void handler(int sigNum );
		int setSigMasked (bool masked );
};
