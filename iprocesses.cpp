#include "iprocesses.h"
extern CCI* gCCI;
extern RTX* gRTX;
extern Queue* gUserInputs;
extern int gRunTime;
extern inputBuffer* gRxMemBuf;
extern inputBuffer* gTxMemBuf;

void i_timing_process()
{	
	static Queue* waitingProcesses = new Queue(Queue::MSG_ENV); //internal Q

	//overall rtx clock count used for trace buffer time stamp
	gRunTime ++;

	//retrieve PCB of currently excecuting process (i_timing_process) 
	PCB* tempPCB;
	assure(gRTX->getCurrentPcb(&tempPCB) == EXIT_SUCCESS,"Failed to retrieve current PCB",__FILE__,__LINE__,__func__,false);
	
	MsgEnv* tempMsg = tempPCB->retrieve_mail();
//	if (tempMsg != NULL)
//	{
//		call sorted EnQ() function in queue class
//		tempMsg = NULL;
//	}
	if (waitingProcesses->get_front()->getTimeStamp() == gRunTime) 
	{
		tempMsg = waitingProcesses->dequeue_MsgEnv();
		tempMsg->setMsgType(MsgEnv::WAKE_UP);
		int returnAddress = tempMsg->getOriginPid();
		gRTX->K_send_message(returnAddress, tempMsg);
	}
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
	
	PCB* currPcb = NULL;
	char* userMsg;
	if(gRTX->getCurrentPcb(&currPcb) == EXIT_SUCCESS) //current PCB is valid
	{
		//extract information from shared memory
		if(gRxMemBuf->data[0] != '\0') //ensure first character isn't a null, i.e. empty command
		{
			userMsg = gRxMemBuf->data;
			string s_UserMsg = userMsg;
			debugMsg("Keyboard input was: ");
			debugMsg(s_UserMsg);
			gRxMemBuf->busyFlag = 0; //indicate that contents of buffer have been copied, data array may be overwritten
			gCCI->userInputs->enqueue(&s_UserMsg); 
		}
	}
	return;
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
	MsgEnv* retMsg;
	int invoker;
	
	PCB* currPcb = NULL;
	if(gRTX->getCurrentPcb(&currPcb) == EXIT_SUCCESS && (*currPcb).check_mail() > 0) //current PCB is valid && Someone is trying to send chars to the console
	{
			retMsg = gRTX->K_receive_message(); //won't be null because already checked if mailbox was empty
			string msgToConsole = retMsg->getMsgData();
			if(retMsg == NULL || msgToConsole == "") //make the check anyways
			{				
				retMsg = NULL;
		   	gRTX->K_send_message(getpid(), retMsg); //send a NULL envelope if there's an error
				return;
			}
			
			invoker = retMsg->getOriginPid();				
			
			//if busy, return fail 
			if(gTxMemBuf->busyFlag == 1) //CRT is busy (currently transmitting something to , cannot output to screen
			{
				retMsg->setMsgType(retMsg->DISPLAY_FAIL);
			}	
			else //CRT is NOT busy - perform transmission
			{
				if(msgToConsole.size() > MAXDATA) //buffer would overflows
				{
					//don't bother copying message into buffer; partial messages are not acceptable. Invoking process must do it line by line
					retMsg->setMsgType(retMsg->BUFFER_OVERFLOW);
				}
				else
				{
					gTxMemBuf->busyFlag = 1; //set buffer to be busy because we're about to transmit something
					int indexInBuf = 0; //start writing from beginning of the shmem
					for(unsigned int i = 0; i < msgToConsole.size(); i++) //copy message to shared memory
					{
						gTxMemBuf->data[indexInBuf] = msgToConsole[i];
						indexInBuf++;
					}
					
					//CRT process will set busyFlag back to 0 once it has taken everything out of the buffer
				    //So can assume that once things are in the buffer, they have been "successfully transmitted"
					retMsg->setMsgType(retMsg->DISPLAY_ACK); 
				}
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
