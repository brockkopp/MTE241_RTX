#include "SignalHandler.h"

#define ANGTEST 0

extern RTX* gRTX;

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
			if(ANGTEST) debugMsg("Atomic(on)",0,1);
			sigprocmask(SIG_BLOCK, &_sigSetBlocked, NULL);
		}
		else
		{
			if(ANGTEST) debugMsg("Atomic(off)",0,1);
			sigprocmask(SIG_SETMASK, &_sigSetHandled, NULL);
		}
	}
	catch(int e	)
	{
		ret = EXIT_ERROR;
	}

	return ret;
}

void SignalHandler::handler( int sigNum )
{

	int prevProc = gRTX->getCurrentPid();
	
	//-1 Fatal since no process's stack to run on
	assure(prevProc >= 0, "No Process on CPU during i_process call. Sig(" + intToStr(sigNum) + ")",__FILE__,__LINE__,__func__,true);

	gRTX->_scheduler->setProcessState(prevProc,READY);
	
	switch(sigNum)
	{
		case SIGINT:
			die(0);		//Signal number 0 denotes normal termination (not forced)
			break;

		case SIGALRM:
			gRTX->_scheduler->setProcessState(PROC_TIMING,EXECUTING);
			gRTX->setCurrentPcb(PROC_TIMING);
			i_timing_process();
			gRTX->_scheduler->setProcessState(PROC_TIMING,READY);
			break;

		case SIGUSR1:	//Keyboard
			gRTX->_scheduler->setProcessState(PROC_KB,EXECUTING);
			gRTX->setCurrentPcb(PROC_KB);
			i_keyboard_handler();
			gRTX->_scheduler->setProcessState(PROC_KB,READY);
			break;

		case SIGUSR2:	//Crt
			gRTX->_scheduler->setProcessState(PROC_CRT,EXECUTING);
			gRTX->setCurrentPcb(PROC_CRT);
			i_crt_handler();
			gRTX->_scheduler->setProcessState(PROC_CRT,READY);
			break;

		default:
			assure(false,"Unknown Signal Received",__FILE__,__LINE__,__func__,false);
			break;			
	}
	gRTX->_scheduler->setProcessState(prevProc,EXECUTING);
	gRTX->setCurrentPcb(prevProc);
}
