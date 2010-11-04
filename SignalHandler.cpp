#include "SignalHandler.h"

SignalHandler::SignalHandler()
{
	debugMsg("Signal Handler Initializing...",0,0);
	sigset( SIGINT 	,die);
	sigset( SIGBUS	,die);
	sigset( SIGHUP	,die);
	sigset( SIGILL	,die);
	sigset( SIGQUIT	,die);
	sigset( SIGABRT	,die);
	sigset( SIGTERM	,die);
	sigset( SIGSEGV	,die);
	sigset( SIGUSR2	,SignalHandler::handler); 
	sigset( SIGUSR1	,SignalHandler::handler); 
	sigset( SIGALRM	,SignalHandler::handler);

	sigemptyset(&_sigSetBlocked);
	sigaddset(&_sigSetBlocked, SIGINT);
	sigaddset(&_sigSetBlocked, SIGALRM);
	sigaddset(&_sigSetBlocked, SIGUSR1);
	sigaddset(&_sigSetBlocked, SIGUSR2);

	sigprocmask(SIG_BLOCK, &_sigSetBlocked, &_sigSetHandled);	

	debugMsg("Done",0,1);
	debugMsg("Signals Masked(init)",0,1);
}

int SignalHandler::setSigMasked(bool masked)
{
	int ret = 1;
	try
	{
		if(masked)
		{
			sigprocmask(SIG_BLOCK, &_sigSetBlocked, NULL);
			debugMsg("Atomic(on)",0,1);
		}
		else
		{
			sigprocmask(SIG_SETMASK, &_sigSetHandled, NULL);
			debugMsg("Atomic(off)",0,1);
		}
	}
	catch(char* str)
	{
		ret = -1;
	}

	return ret;
}

int SignalHandler::atomic(bool on){
/*	int ret = EXIT_SUCCESS;
	int * atmCnt = rtx->schedler->cp->atomicNumber;	
	
	(on) ? atmCnt++ : atmCnt --;
	
	if(atmCnd < 0)
		ret = EXIT_ERROR;
	if(atmCnt == 0)
		sigSetMasked(false);
	if(atmCnt == 1)
		sigSetMasked(true);

	return ret;
*/
	return -2;
}

void SignalHandler::handler( int sigNum )
{
	
}

