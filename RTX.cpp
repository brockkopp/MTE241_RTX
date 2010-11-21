#include "RTX.h"
extern CCI* gCCI;

RTX::RTX(PcbInfo* initTable[], SignalHandler* signalHandler)
{

	debugMsg("RTX Initializing...",0,1);
	//Inititalize RTX members, each cascades to its own constructor which performs memory allocation
	_signalHandler = signalHandler;

	//Initialize each PCB from init table
	for(int i=0; i < PROCESS_COUNT; i++)
		_pcbList[i] = new PCB(initTable[i]);

	/* Transfer the _pcbList into a queue for use with the scheduler... 
	*/
	
	Queue* pcbTmpList = new Queue(Queue::PROCCONBLOCK); //Init queue of PCBs
	
	//Loop through _pcbList, enqueue each item into pcbTmpList.
	for(int i=0; i < PROCESS_COUNT; i++)
		pcbTmpList->enqueue(_pcbList[i]);
	
	_scheduler = new Scheduler (pcbTmpList);
	delete pcbTmpList;
	
	_signalHandler->setSigMasked(false);

	debugMsg("RTX Init Done",0,1);
}

RTX::~RTX()
{
	_signalHandler->setSigMasked(true);
	//Free resources held by each RTX member, allocated in the RTX constructor

	//delete _mailMan;
	//delete _scheduler;
	//delete _signalHandler;
}

//assure(gRTX->getCurrentPcb(&tempPCB) == EXIT_SUCCESS,"Failed to retrieve PCB",__FILE__,__LINE__,__func__,false);
int RTX::getPcb(int pid, PCB** pcb)
{
	int ret = EXIT_SUCCESS;

	if(pid >= 0 && pid < PROCESS_COUNT)
		*pcb = _pcbList[pid];
	else
		ret = EXIT_ERROR;
	
	return ret;
}

//assure(gRTX->getCurrentPcb(&tempPCB) == EXIT_SUCCESS,"Failed to retrieve current PCB",__FILE__,__LINE__,__func__,false);
int RTX::getCurrentPcb(PCB** pcb)
{
	int ret = EXIT_SUCCESS;
	
	if(_scheduler == NULL || (*pcb = _scheduler->get_current_process()) == NULL)
		ret = EXIT_ERROR;

	return ret;
}

int RTX::atomic(bool on)
{
	int ret = EXIT_SUCCESS;
	PCB* currPcb = NULL;
	
	if(_scheduler == NULL)			//TESTING ONLY!!!!!!!!!!!!!!!
	{
		debugMsg("no scheduler... forcing atomic",0,1);
		_signalHandler->setSigMasked(on);
	}
	else if(assure(getCurrentPcb(&currPcb) == EXIT_SUCCESS,"Failed to retrieve current PCB",__FILE__,__LINE__,__func__,false))
	{	
		int cnt = (on) ? currPcb->incAtomicCount() : currPcb->decAtomicCount();
		
		if (cnt == 0)				
			_signalHandler->setSigMasked(false);
		else if(cnt == 1)
			_signalHandler->setSigMasked(true);
	}
	else
		ret = EXIT_ERROR;
			
	return ret;
}

//Call MsgServ class function sendMsg
int RTX::K_send_message(int dest_process_id, MsgEnv* msg_envelope)
{
	return _mailMan->sendMsg(dest_process_id, msg_envelope);
}

//Call MsgServ class function recieveMsg
MsgEnv* RTX::K_receive_message()
{
	return _mailMan->recieveMsg();
}

//Call MsgServ class function requestEnv
MsgEnv* RTX::K_request_msg_env()
{	
	return _mailMan->requestEnv();
}

//Call MsgServ class function releaseEnv
int RTX::K_release_msg_env(MsgEnv* memory_block)
{
	return _mailMan->releaseEnv(memory_block);
}

int RTX::K_release_processor()
{
	//We need a scheduler object names scheduler to be declared (in initialization???)
	
	//return scheduler.release_processor();
	return -2;
}

int RTX::K_request_process_status(MsgEnv* memory_block) 
{
//	array returnArray
//	for each PCB
//	{
//		status = PCB.get_status
//		priority = PCB.get_priority
//		Add status and priority to returnArray
//	}
//	Put returnArray into message contents
//	send_message( invoking_process, msg_env_ptr )
//	;
return -2;
	
}

int RTX::K_terminate()
{
	//Execute final cleanup and program termination, 0 denotes normal exit
	die(0);
	return 1;
}

int RTX::K_change_priority(int new_priority, int target_process_id)
{
	//We need a scheduler object names scheduler to be declared (in initialization???)
	
	//return scheduler.change_priority(new_priority,target_process_id);
	
	return -2;
}

int RTX::K_request_delay(int time_delay, int wakeup_code, MsgEnv* msg_envelope)
{
	
	if(msg_envelope != NULL)
	{
		//populate msg env Fields
		msg_envelope->setTimeStamp(time_delay); 
		msg_envelope->setMsgType(msg_envelope->WAKE_UP);
		//call Kernal send message to send to timing iProcess
		return K_send_message(0, msg_envelope); //i_timing_process PID is 0
	}
	return EXIT_ERROR;
}

/* Message envelope contains messages (character string) to sent to console. 
 * String must be in usual C/C++ string format terminated by null character
 * send_console_chars sends then adds the message into the message envelope, and then sends the envelope to i_crt_handler 
 * send_console_chars then sends a signal to i_crt_handler who outputs to the console
 * After tranmission is complete, the same envelope is returned to invoking process with message_type "display_ack" as confirmation
 * Inovking process does not block! So if the CRT is busy, will return EXIT_ERROR and invoking process must loop to ensure transmission is complete
 * Returns EXIT_SUCCESS if successful, EXIT_ERROR otherwise (eg. if message not terminated with null char or transmission fails */
int RTX::K_send_console_chars(MsgEnv* msg_envelope)
{
	kill(getpid(), SIGUSR2);
//	if(msg_envelope == NULL) //error check
//		return EXIT_ERROR;
//		
//	string toSend = msg_envelope->getMsgData();
//	if(toSend[toSend.length()-1] != '\0') //ensure message is terminated by null character	
//		return EXIT_ERROR;
//	
//	//validated that message is in correct format
//	int iCRTProcId = getpid(); //send a signal to the RTX
//	int invoker = msg_envelope->getOriginPid();
//	//send message to i_crt_handler to deal with transmission of the message to the console
//	msg_envelope->setMsgType(msg_envelope->TRANSMIT_TO_CRT_REQUEST);
//	int res = K_send_message(iCRTProcId, msg_envelope);
//	
//	if(res != EXIT_ERROR)
//	{
//		//make a copy of the current mailbox, then empty it so can receive message from iprocesses without hassle
//		PCB* curr = NULL;
//		getCurrentPcb(&curr);
//		Queue* temp = curr->copy_mailbox();
//		curr->empty_mailbox();
//		
//		kill(iCRTProcId, SIGUSR2); //send signal to i_crt_handler who will handle transmitting the message
//	  	//this is a blocking call, but not really since the i_crt_process runs to completion after the signal is sent, and the i_crt_handler sends a message before exiting	  
//	  	msg_envelope = K_receive_message(); 
//	  
//	 	curr->set_mailbox(temp); //restore mailbox
//	  
//		bool transmission_failed = (msg_envelope == NULL);
//		if(!transmission_failed)
//		{
//			if(msg_envelope->getMsgType() == msg_envelope->BUFFER_OVERFLOW || msg_envelope->getMsgType() == msg_envelope->DISPLAY_FAIL)
//			{
//				res = EXIT_ERROR;
//				K_send_message(invoker, msg_envelope);
//			}
//			else //display_ack
//				res = K_send_message(invoker, msg_envelope); //the message type was set to DISPLAY_ACK by the iprocess
//		}
//		else
//		{
//			res = EXIT_ERROR;
//			K_send_message(invoker, msg_envelope);
//		}
//	}
//	return res;
	return -2;
}

/* Invoking process provides a message envelope (previously allocated)
 * A message is sent to the invoking process using the envelope provided once the end of line character is received or the buffer is filled. 
 * Message type is "console_input"; message contains characters received
 * End of keyboard string is indicated by null character
 * Returns EXIT_SUCCESS if successful, EXIT_ERROR otherwise (i.e. no characters waiting) */
int RTX::K_get_console_chars(MsgEnv* msg_envelope)
{
	int res;
	if(atomic(true))
	{	
		int invoker = msg_envelope->getOriginPid();
		if(gCCI->userInputs->get_length() == 0) //no user input is available
		{
		  	msg_envelope->setMsgData("");
		  	msg_envelope->setMsgType(msg_envelope->NO_INPUT);
		  	K_send_message(invoker, msg_envelope);
		  	res = EXIT_ERROR;
		}
		else
		{
			msg_envelope->setMsgData(*(gCCI->userInputs->dequeue_string()));
			msg_envelope->setMsgType(msg_envelope->CONSOLE_INPUT);			
			res = K_send_message(invoker, msg_envelope);
		}
	}
	atomic(false);
	return res;
}

int RTX::K_get_trace_buffers(MsgEnv* msg_envelope)
{
	//return _msgTrace->getTraces(); 
	return -2;
}
