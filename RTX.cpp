#include "RTX.h"

RTX::RTX(PcbInfo* initTable[], SignalHandler* signalHandler)
{
	debugMsg("RTX Initializing...",0,1);
	//Inititalize RTX members, each cascades to its own constructor which performs memory allocation
	_signalHandler = signalHandler;
	_scheduler = NULL;
	_msgTrace = NULL;																					//added by Eric, allows mailMan access to trace functions
	_mailMan = new MsgServ(_scheduler, _msgTrace);						//added by Eric, allows mailMan access to functions

	//Initialize each PCB from init table
	for(int i=0; i < PROCESS_COUNT; i++)
		_pcbList[i] = new PCB(initTable[i]);

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
	
	if(_scheduler == NULL)			//TESTING ONLY!!!!!!!!!!!!!!!
	{
		*pcb = _pcbList[0];
		debugMsg("Default pcb used in getCurrentPcb",0,1);
	}
	else if(_scheduler == NULL || (*pcb = _scheduler->get_current_process()) == NULL)
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

int RTX::K_send_message(int dest_process_id, MsgEnv* msg_envelope)
{
	return _mailMan->sendMsg(dest_process_id, msg_envelope);
}

MsgEnv* RTX::K_receive_message()
{
	return _mailMan->recieveMsg();
}

MsgEnv* RTX::K_request_msg_env()
{
	return _mailMan->requestEnv();
}

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

int RTX::K_request_process_status(MsgEnv* memory_block) //why does this requre a msg envelope? shouldn't it just require the PCB and return the status? -Eric
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
		msg_envelope->setTimeStamp(time_delay); 
		msg_envelope->setMsgType(msg_envelope->WAKE_UP);
		//need to have the PID of the timeing_Iprocess, #define I_TIMING_PID? Q*Q*Q*Q*Q*Q*Q*Q*Q*Q*Q
		//return K_send_message(I_TIMING_PID, msg_envelope);
	}
	return -2;
}

/* Message envelope contains messages (character string) to sent to console. 
 * String must be in usual C/C++ string format terminated by null character
 * send_console_chars sends then message onto the i_crt_handler who then deals with outputting to the console
 * After tranmission is complete, the same envelope is returned to invoking process with message_type "display_ack" as confirmation
 * Inovking process does not block! 
 * Returns EXIT_SUCCESS if successful, EXIT_ERROR otherwise (eg. if message not terminated with null char or transmission times out */
int RTX::K_send_console_chars(MsgEnv* msg_envelope)
{
	if(msg_envelope == NULL) //error check
		return EXIT_ERROR;
		
	string toSend = (*msg_envelope).getMsgData();
	if(toSend[toSend.length()-1] != '\n') //ensure message is terminated by null character	
		return EXIT_ERROR;
	
	//validated that message is in correct format
	int iCRTProcId = -2;
	int invoker = (*msg_envelope).getOriginPid();
	//send message to i_crt_handler to deal with transmission of the message to the console
	(*msg_envelope).setMsgType((*msg_envelope).TRANSMIT_TO_CRT_REQUEST);
	int res = K_send_message(iCRTProcId, msg_envelope);
	
	if(res != EXIT_ERROR)
	{
		kill(iCRTProcId, SIGUSR2); //send signal to i_crt_handler who will handle transmitting the message
	
		bool transmission_complete = false;
		int failCount = 0;
		while(!transmission_complete && failCount < 10) //should this loop be done here or in i_process? ANG
		{
			msg_envelope = _mailMan->recieveMsg(); //this is blocking call!
			if(msg_envelope->getMsgType() == (*msg_envelope).DISPLAY_ACK)
			{
				transmission_complete = true;
				res = K_send_message(invoker, msg_envelope);
			}			
			else
				failCount++;
		}
		if(msg_envelope->getMsgType() == (*msg_envelope).DISPLAY_FAIL)
			res = K_send_message(invoker, msg_envelope);
			
		if(!transmission_complete) //i.e. operation timed out
			return EXIT_ERROR;
	}
	return res;
}

/* Invoking process provides a message envelope (previously allocated) - NON BLOCKING!
 * A message is sent to the invoking process using the envelope provided once the end of line character is received or the buffer is filled. 
 * Message type is "console_input"; message contains characters received
 * End of keyboard string is indicated by null character
 * Returns EXIT_SUCCESS if successful, EXIT_ERROR otherwise (i.e. no characters waiting) */
int RTX::K_get_console_chars(MsgEnv* msg_envelope)
{
//	if(gUserInputs->get_length() == 0)
//		return EXIT_ERROR;
//	
//	int invoker = (*msg_envelope).getOriginPid();
//	(*msg_envelope).setMsgData(*(gUserInputs.dequeue_string()));
//	(*msg_envelope).setMsgType((*msg_envelope).CONSOLE_INPUT);
//	return K_send_message(invoker, msg_envelope);
	return -2;
}

int RTX::K_get_trace_buffers(MsgEnv* msg_envelope)
{
	//return _msgTrace->getTraces(); // waiting on approval of _msgTrace in RTX.h private members - Eric
	return -2;
}
