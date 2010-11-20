#include "iprocesses.h"
extern CCI* gCCI;
extern RTX* gRTX;
extern Queue* gUserInputs;

void i_timing_process()
{
//	//overall clock count used for trace buffer time stamp
//	static int timeCount = 0;//make global, put it in file where it has been decided to place global variables
//	
//	static Queue* waitingProcesses; //internal Q
//	static int tickCount = 0; // used to determine when a delay request is expired
//	
//	timeCount++;
//	tickCount++;
//	
//	//retrieve PCB of currently excecuting process (i_timing_process) 
//	PCB* tempPCB = get_current_process();
//		
//	MsgEnv* tempMsg = tempPCB->retrieve_mail();
//	if (tempMsg != NULL)
//	{
//		//compare time_delay with those in the internal Q
//		//insert msg into correct place, subtract the time_delay request o all msg infrount of this one
//		tempMsg = NULL;
//	}
//	if (waitingProcesses->_front->_timeDelay == tickCount) //if we add the timeDelay private member
//	{
//		tempMsg = waitingProcesses.dequeue_MsgEnv();
//		tempMsg.setMsgType(WAKE_UP);
//		int returnAddress = tempMsg->_originPid;
//		sendMsg(returnAddress, tempMsg);
//		tickCount = 0; 
//	}
//			
//	gCCI->wallClock->increment();

//	string time;
//	if((time = gCCI->wallClock->toString()) != "")
//		cout << time << endl;

	ualarm(0,0);
	
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
		gUserInputs->enqueue(&userMsg);
	}
	return;
	
			/*void kbd_handler(int signum)
		{
		inputbuf command;
		// copy input buffer
		if (in_mem_p->indata[0] != '\0')
		{
		strcpy(command.indata,in_mem_p->indata);
		// we should parse the input string and execute the command given,
		// but for now we just echo the input
		//
		printf("Keyboard input was: %s\n",command.indata);
		in_mem_p->ok_flag = 0; // tell child that the buffer has been emptied
		}
		}
		*/
}

/* Signal is sent to the i_crt_handler from the K_send_console_chars primitive. 
 * Before sending the signal, K_send_console_chars also sends the i_crt_handler a message envelope containing the message to be transmitted to the crt
 * The i_crt_handler therefore takes the message from its mailbox and adds it to the shared memory. Once added to shmem, the message is as good as sent.
 * The i_crt_handler WILL NOT write to the shmem if the CRT process is busy
 * The i_crt_handler will send a message envelope before exiting; envelope will be NULL if there's an error
 * If the transmission completes successfully, i_crt_handler will return an acknowledgement envelope */
void i_crt_handler()
{
	debugMsg("Signal Received: SIGUSR2: CRT",0,1);
	bool CRTisBusy = false;
	MsgEnv* retMsg;
	int invoker;
	
	PCB* currPcb = NULL;
	if(gRTX->getCurrentPcb(&currPcb) == EXIT_SUCCESS && (*currPcb).check_mail() > 0) //current PCB is valid && Someone is trying to send chars to the console
	{
			retMsg = gRTX->K_receive_message(); //won't be null because already checked if mailbox was empty
			if(retMsg == NULL) //make the check anyways
			{				
		   	gRTX->K_send_message(getpid(), retMsg); //send a NULL envelope if there's an error
				return;
			}
			
			invoker = (*retMsg).getOriginPid();				
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
	}
	else //an error occurred, send a NULL envelope
	{
		retMsg = NULL;
		invoker = getpid();
	}
	gRTX->K_send_message(invoker, retMsg);	
	return;
}
