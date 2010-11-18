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

/* Signal is sent to the i_keyboard_handler from the Keyboard process. 
 * I_KB reads the data from the shmem and adds it to a queue of user input (global variable)
 * K_get_console_chars extracts user inputs from the global queue as necessary */
void i_keyboard_handler()
{
	debugMsg("Signal Received: SIGUSR1: KB",0,1);
	bool KEYBOARDisBusy = false; //!!!*!*!*!*!*!*!*QQQQQQ*~!*!*!*!*!* Why do I need this? ~Ang
	
	PCB* currPcb = NULL;
	if(gRTX->getCurrentPcb(&currPcb) == EXIT_SUCCESS && !KEYBOARDisBusy) //current PCB is valid
	{
		//extract information from shared memory
		string userMsg = "";		
		//gUserInputs->enqueue(&userMsg);
	}
	return;
}

/* Signal is sent to the i_crt_handler from the K_send_console_chars primitive. 
 * Before sending the signal, K_send_console_chars also sends the i_crt_handler a message envelope containing the message to be transmitted to the crt
 * The i_crt_handler therefore takes the message from its mailbox and adds it to the shared memory. Once added to shmem, the message is as good as sent.
 * The i_crt_handler WILL NOT write to the shmem if the CRT process is busy
 * If the transmission is complete, i_crt_handler will return the envelope to the invoking process as acknowledgement */
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
				(*retMsg).setMsgType((*retMsg).DISPLAY_ACK);
			}
			else //return message that transmission failed
			{
				(*retMsg).setMsgType((*retMsg).DISPLAY_FAIL);
			}		
			gRTX->K_send_message(invoker, retMsg);	
		}
	}
	return;
}
