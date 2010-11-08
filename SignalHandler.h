#ifndef H_SIGHANDLER
#define H_SIGHANDLER

#include "debug.h"
#include "signal.h"
#include "iprocesses.h"

class SignalHandler
{
	private:
		sigset_t _sigSetBlocked;
		sigset_t _sigSetHandled;
	
	public:
		SignalHandler();
		static void handler(int sigNum );
		int setSigMasked (bool masked );
		int atomic(bool on);
};
#endif
