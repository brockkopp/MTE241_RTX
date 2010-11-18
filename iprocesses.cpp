#include "iprocesses.h"
extern CCI* gCCI;
extern RTX* gRTX;

void i_timing_process()
{
	//int timeCount;//make global, put it in file where it has been decided to place global variables
	//timeCount++;
	//check pcb msg q for new msgs
		//insert any new msgs into the internal q
	//check if any msgs on the internal q are expired
		//if so send msgs to the process and wake it up
	
	gCCI->wallClock->increment();

	string time;
	if((time = gCCI->wallClock->toString()) != "")
		cout << time << endl;

	//ualarm(0,0);
	
	return;
}
void i_keyboard_handler()
{
	debugMsg("Signal Received: SIGUSR1: KB",0,1);
	return;
}
void i_crt_handler()
{
	debugMsg("Signal Received: SIGUSR2: CRT",0,1);
	bool CRTisBusy = false;
	
	PCB* currPcb = NULL;
	if(gRTX->getCurrentPcb(&currPcb) == EXIT_SUCCESS) //current PCB is valid
	{
		if((*currPcb).check_mail() > 0 ) //Someone is trying to send chars to the console
		{
			MsgEnv* retMsg = gRTX->K_receive_message(); //won't be null because already checked if mailbox was empty
			if(retMsg == NULL) //make the check anyways
			{
				assure(true,"Null envelope in I_CRT mailbox. Cannot transmit, don't know who I'm supposed to tattle on!",__FILE__,__LINE__,__func__,true);
				return;
			}
			
			int invoker = (*retMsg).getOriginPid();				
			if(!CRTisBusy) //CRT is NOT busy - perform transmission
			{
				CRTisBusy = true;
				//add message to shmem
				//what if have to wait for crt process?
			}
			else //return message that transmission failed
			{
				(*retMsg).setMsgType((*retMsg).DISPLAY_FAIL);
				gRTX->K_send_message(invoker, retMsg);			
			}		
		}
		return;
	}
}
