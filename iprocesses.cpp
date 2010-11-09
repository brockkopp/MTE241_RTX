#include "iprocesses.h"
//extern RTX* gRTX;
//extern CCI* gCCI;

void i_timing_process()
{
	//gCCI->wallClock->increment();
}
void i_keyboard_handler()
{
	debugMsg("Signal Received: SIGUSR1: KB",0,1);
	return;
}
void i_crt_handler()
{
	debugMsg("Signal Received: SIGUSR2: CRT",0,1);
	return;
}


