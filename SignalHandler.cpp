#include "SignalHandler.h"

SignalHandler::SignalHandler()
{
	debugMsg("Signal Handler Initializing...",0,0);
	//Initialize default signal set
	sigset( SIGBUS	,die);
	sigset( SIGHUP	,die);
	sigset( SIGILL	,die);
	sigset( SIGQUIT	,die);
	sigset( SIGABRT	,die);
	sigset( SIGTERM	,die);
	sigset( SIGSEGV	,die);
	sigset( SIGINT 	,SignalHandler::handler);
	sigset( SIGUSR1	,SignalHandler::handler); 
	sigset( SIGUSR2	,SignalHandler::handler); 
	sigset( SIGALRM	,SignalHandler::handler);

	//Initialize blocked signal set
	sigemptyset(&_sigSetBlocked);
	sigaddset(&_sigSetBlocked, SIGINT);
	sigaddset(&_sigSetBlocked, SIGALRM);
	sigaddset(&_sigSetBlocked, SIGUSR1);
	sigaddset(&_sigSetBlocked, SIGUSR2);

	//Set blocked signal set to current set as well as store default signal set to _sigSetHandled
	sigprocmask(SIG_BLOCK, &_sigSetBlocked, &_sigSetHandled);	

	debugMsg("Done",0,1);
	debugMsg("Signals Masked(init)",0,1);
}

int SignalHandler::setSigMasked(bool masked)
{
	int ret = EXIT_SUCCESS;
	try
	{
		if(masked)
		{
			debugMsg("Atomic(on)",0,1);
			sigprocmask(SIG_BLOCK, &_sigSetBlocked, NULL);
		}
		else
		{
			debugMsg("Atomic(off)",0,1);
			sigprocmask(SIG_SETMASK, &_sigSetHandled, NULL);
		}
	}
	catch(char* str)
	{
		ret = EXIT_ERROR;
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
	debugMsg("Signal Received: ");
	switch(sigNum)
	{
		case SIGINT:
			debugMsg("SIGINT",0,1);
			die(EXIT_SUCCESS);
			break;

		case SIGALRM:
			debugMsg("SIGALRM",0,1);
			break;

		case SIGUSR1:	//Keyboard
			debugMsg("SIGUSR1: KB",0,1);
			break;

		case SIGUSR2:	//Crt
			debugMsg("SIGUSR2: CRT",0,1);
			break;
		default:
			assure(false,"Unknown Signal Received",__FILE__,__LINE__,__func__,false);
			break;			
	}
}
