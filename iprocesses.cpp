#include "iprocesses.h"

void i_timing_process()
{
	//increase count, call wall clock, check msgs, etc
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


