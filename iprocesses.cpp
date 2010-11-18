#include "iprocesses.h"
extern CCI* gCCI;
extern RTX* gRTX;

void i_timing_process()
{/*
	//overall clock count used for trace buffer time stamp
	static int timeCount = 0;//make global, put it in file where it has been decided to place global variables
	
	static Queue* waitingProcesses; //internal Q
	static int tickCount = 0; // used to determine when a delay request is expired
	
	timeCount++;
	tickCount++;
	
	//retrieve PCB of currently excecuting process (i_timing_process) 
	PCB* tempPCB = get_current_process();
		
	MsgEnv* tempMsg = tempPCB->retrieve_mail();
	if (tempMsg != NULL)
	{
		//compare time_delay with those in the internal Q
		//insert msg into correct place, subtract the time_delay request o all msg infrount of this one
		tempMsg = NULL;
	}
	if (waitingProcesses->_front->_timeDelay == tickCount) //if we add the timeDelay private member
	{
		tempMsg = waitingProcesses.dequeue_MsgEnv();
		tempMsg.setMsgType(WAKE_UP);
		int returnAddress = tempMsg->_originPid;
		sendMsg(returnAddress, tempMsg);
		tickCount = 0; 
	}
			
	gCCI->wallClock->increment();
	string time;
	if((time = gCCI->wallClock->toString()) != "")
	{
		//print time
	}
	return;
*/}
void i_keyboard_handler()
{
	debugMsg("Signal Received: SIGUSR1: KB",0,1);
	return;
}
void i_crt_handler()
{/*
	debugMsg("Signal Received: SIGUSR2: CRT",0,1);
	bool CRTisBusy = false;
	
	PCB* currPcb = NULL;
//	if(getCurrentPcb(&currPcb) == EXIT_SUCCESS) HOW DO I USE GETCURRENTPCB!? NEED AN INSTANCE OF THE RTX!!!
	if(true)//current PCB is valid
	{
		if((*currPcb).check_mail() > 0 ) //Someone is trying to send chars to the console
		{
			MsgEnv* retMsg = ((*currPcb).retrieve_mail()); //won't be null because already checked if mailbox was empty
			int invoker = (*currPcb).get_id();
				
			if(!CRTisBusy) //CRT is NOT busy - perform transmission
			{
				CRTisBusy = true;
				//add message to shmem
				//what if have to wait for crt process?
			}
			else //return message that transmission failed
			{
				//(*retMsg).setMsgType((*retMsg).DISPLAY_FAIL);
				(*retMsg).setMsgType(MsgEnv::DISPLAY_FAIL);
				//K_send_message(invoker, retMsg);				CANT DO THIS WITHOUT INSTANCE OF RTX!!!!
				return;
			}		
		}
		else //receieved a signal
		{
			CRTisBusy = false;
			//how do I return the envelope if there's none in the mailbox? need while loop above?
		}
	}*/	
	return;
}
