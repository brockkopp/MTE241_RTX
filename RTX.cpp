#include "RTX.h"

RTX::RTX(PcbInfo* initTable[], SignalHandler* signalHandler)
{
	debugMsg("RTX Initializing...",0,1);
	//Inititalize RTX members, each cascades to its own constructor which performs memory allocation
	_signalHandler = signalHandler;
	_scheduler = NULL;

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

int RTX::getPcb(int pid, PCB** pcb)
{
	int ret = EXIT_SUCCESS;

	if(pid >= 0 && pid < PROCESS_COUNT)
		*pcb = _pcbList[pid];
	else
		ret = EXIT_ERROR;
	
	return ret;
}

int RTX::getCurrentPcb(PCB** pcb)
{
	int ret = EXIT_SUCCESS;

	if(_scheduler != NULL && _scheduler->get_current_process() != NULL)
		*pcb = _scheduler->get_current_process();
	else
		ret = EXIT_ERROR;	
	
	return ret;
}

int RTX::atomic(bool on)
{
	int ret = EXIT_SUCCESS;
	PCB* currPcb = NULL;
	
	if(getCurrentPcb(&currPcb) != EXIT_SUCCESS)
		ret = EXIT_ERROR;
	else
	{	
		int cnt = (on) ? currPcb->incAtomicCount() : currPcb->decAtomicCount();

		if (cnt == 0)				
			_signalHandler->setSigMasked(false);
		else if(cnt == 1)
			_signalHandler->setSigMasked(true);
	}

	return ret;
}

int RTX::K_send_message(int dest_process_id, MsgEnv* msg_envelope)
{
	return sendMsg(dest_process_id, msg_envelope);
}

MsgEnv* RTX::K_receive_message()
{
	return recieveMsg();
}

MsgEnv* RTX::K_request_msg_env()
{
	return requestEnv();
}

int RTX::K_release_msg_env(MsgEnv* memory_block)
{
	return releaseEnv(memory_block);
}

int RTX::K_release_processor()
{
	return -2;
}

int RTX::K_request_process_status(MsgEnv* memory_block) //why does this requre a msg envelope? shouldn't it just require the pid to find the PCB? -Eric
{
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
	return -2;
}

int RTX::K_request_delay(int time_delay, int wakeup_code, MsgEnv* msg_envelope)
{
	if(msg_envelope != NULL)
	{
		//msg_envelope->setMsgData(time_delay);
		
	}
	return -2;
}

/* Message envelope contains messages (character string) to sent to console. 
 * String must be in usual C/C++ string format terminated by null character
 * send_console_chars sends then message onto the i_crt_handler who then deals with outputting to the console
 * After tranmission is complete, the same envelope is returned to invoking process with message_type "display_ack" as confirmation
 * Inovking process does not block! 
 * Returns EXIT_SUCCESS if successful, EXIT_ERROR otherwise (eg. if message not terminated with null char */
int RTX::K_send_console_chars(MsgEnv* msg_envelope)
{
	if(msg_envelope == NULL) //error check
		return EXIT_ERROR;
		
	string toSend = (*msg_envelope).getMsgData();
	if(toSend[toSend.length()-1] != '\n') //ensure message is terminated by null character	
		return EXIT_ERROR;
	
	//validated that message is in correct format
	int iCRTProcId = -2;
	//send message to i_crt_handler to deal with transmission of the message to the console
	int res = K_send_message(iCRTProcId, msg_envelope);
	i_crt_handler();
	
	//verify transmission was successful
	//USE (*msg_envelope).DISPLAY_FAIL;!!!!!!!!!!!!!!!!!!!!!!!!
	if((*msg_envelope).getMsgType() != MsgEnv::DISPLAY_ACK)
		return EXIT_ERROR;
		
	return res;
}

int RTX::K_get_console_chars(MsgEnv* msg_envelope)
{
	return -2;
}

int RTX::K_get_trace_buffers(MsgEnv* msg_envelope)
{
	return -2;
}
