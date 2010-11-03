#include "SignalHandler.h"

SignalHandler::SignalHandler()
{
/*	sigset( SIGINT 	,die);
	sigset( SIGBUS	,die);
	sigset( SIGHUP	,die);
	sigset( SIGILL	,die);
	sigset( SIGQUIT	,die);
	sigset( SIGABRT	,die);
	sigset( SIGTERM	,die);
	sigset( SIGSEGV	,die);*/
	sigset( SIGUSR2	,SignalHandler::handler); 
	sigset( SIGUSR1	,SignalHandler::handler); 
	sigset( SIGALRM	,SignalHandler::handler);
}

void SignalHandler::handler( int sigNum )
{
/*	
	sigaction sa = new sigaction(); 
	act.sa_sigaction = sighandler;
	act.sa_flags = SA_SIGINFO;

	sigaction(SIGTERM, &act, NULL);
*/
}

