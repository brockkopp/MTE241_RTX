#include "RTX.h"

#include <stdio.h>

extern inputBuffer* gRxMemBuf;

RTX::RTX(PcbInfo* initTable[], SignalHandler* signalHandler)
{
	_pcbList = new PCB* [8];
	debugMsg("RTX Initializing...",0,1);
	//Inititalize RTX members, each cascades to its own constructor which performs memory allocation
	_signalHandler = signalHandler;

	Queue* pcbTmpList = new Queue(Queue::PROCCONBLOCK); //Init queue of PCBs
	waitingProcesses = new Queue(Queue::MSG_ENV);
	runTime = 0;

	//Put all processes from the intialize table into the queue to be passed
	//to the scheduler to put on the ready queue. Do not allow i_processes onto
	//this list.	

	for(int i=0; i < PROCESS_COUNT; i++)
	{
		_pcbList[i] = new PCB(initTable[i]);
		if ( _pcbList[i]->getProcessType() != PROCESS_I )
			pcbTmpList->enqueue((void**)(&(_pcbList[i])));
	}
	
	_scheduler = new Scheduler(pcbTmpList);
	delete pcbTmpList;

	setCurrentPcb(PROC_CCI);

	_msgTrace = new MsgTrace();

	_mailMan = new MsgServ(_scheduler, _msgTrace);

	wallClock = new WallClock(TICK_TIME);

	_signalHandler->setSigMasked(false);

	_started = false;
	_semSend = false;

	debugMsg("RTX Init Done",0,2);
}

RTX::~RTX()
{
	_signalHandler->setSigMasked(true);
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

int RTX::setCurrentPcb(PCB* pcb)
{
	int ret = EXIT_SUCCESS;
	
	if(pcb != NULL)
		_currentProcess = pcb;
	else
		ret = EXIT_ERROR;

	return ret;
}

int RTX::setCurrentPcb(int pid)
{
	PCB* tmp;
	getPcb(pid,&tmp);
	return setCurrentPcb(tmp);
}

PCB* RTX::getCurrentPcb()
{
	return _currentProcess;
}

int RTX::getCurrentPid()
{
	int pid = -1;
	PCB* tempPcb;
	if((tempPcb = getCurrentPcb()) != NULL)
		pid = tempPcb->getId();
		
	return pid;
}

int RTX::atomic(bool on)
{
	int ret = EXIT_SUCCESS;
	PCB* currPcb = NULL;

	if(assure((currPcb = getCurrentPcb()) != NULL,"Failed to retrieve current PCB",__FILE__,__LINE__,__func__,false))
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
int RTX::K_send_message(int dest_process_id, MsgEnv* msgEnv)
{
	int ret;
	atomic(true);
	ret = _mailMan->sendMsg(dest_process_id, msgEnv);
	atomic(false);
	return ret;
}

//Call MsgServ class function recieveMsg
MsgEnv* RTX::K_receive_message()
{
	MsgEnv* ret;
	atomic(true);
	ret = _mailMan->recieveMsg();
	atomic(false);
	return ret;
}

//Call MsgServ class function requestEnv
MsgEnv* RTX::K_request_msg_env()
{	
	MsgEnv* ret;
	atomic(true);
	ret = _mailMan->requestEnv();
	atomic(false);
	return ret;
}

//Call MsgServ class function releaseEnv
int RTX::K_release_msg_env(MsgEnv* usedEnv)
{
	int ret = EXIT_SUCCESS;
	if(assure(usedEnv != NULL,"Releasing NULL Env",__FILE__,__LINE__,__func__,false))
	{
		atomic(true);
		ret = _mailMan->releaseEnv(usedEnv);
		atomic(false);
	}
	return ret;
}

int RTX::K_release_processor()
{
	atomic(true);
	_scheduler->release_processor();
	atomic(false);
	return EXIT_SUCCESS;
}

/*
arguments:
	msg must have memory allocaed to it before fxn call;
*/
int RTX::K_request_process_status(MsgEnv* msg) 
{
	int ret = EXIT_ERROR;
	if(msg != NULL)
	{
		atomic(true);
		string output = "\tPID\tPRIORITY  STATUS\n\t---\t------\t  --------\n";
		
		PCB* curr;
		for(int i=0; i < PROCESS_COUNT && getPcb(i,&curr) == EXIT_SUCCESS; i++)
			output += "\t" + intToStr(i) + ":\t" + intToStr(curr->getPriority()) + "\t  " + curr->getStateName() + "\n";

		msg->setDestPid(msg->getOriginPid());		//Waiting on Message implementation
		msg->setMsgData(output);
		ret = EXIT_SUCCESS;
		atomic(false);
	}
	else
		debugMsg("Called K_request_process_status without first allocating memory to the passed MsgEnv\n");
	
	return ret;
}

int RTX::K_terminate()
{
	//Execute final cleanup and program termination, 0 denotes normal exit
	die(0);
	return EXIT_ERROR;		//Should never reach this point
}

int RTX::K_change_priority(int new_priority, int target_process_id)
{
	int return_val;
	
	PCB* tmpPcb;
	if(	(return_val = getPcb(target_process_id, &tmpPcb)) == EXIT_SUCCESS )
	{
		atomic(true);	
		return_val = _scheduler->change_priority( tmpPcb , new_priority);
		atomic(false);
	}
	return return_val;
}

//sends a msg to the i_timing_process with a sleep time
int RTX::K_request_delay(int time_delay, int wakeup_code, MsgEnv* msg_envelope)
{
	int ret = EXIT_ERROR;
	if(msg_envelope != NULL)
	{
		atomic(true);
		//populate msg env Fields
		msg_envelope->setTimeStamp(time_delay);
		msg_envelope->setMsgType(MsgEnv::REQ_DELAY);
		msg_envelope->setMsgData(intToStr(wakeup_code));
		
	//call Kernal send message to send to timing iProcess
		ret = K_send_message(PROC_TIMING, msg_envelope);
		_scheduler->block_process(SLEEPING);
		atomic(false);	
	}
	return ret;
}

/* Message envelope contains messages (character string) to sent to console. 
 * String must be in usual C/C++ string format terminated by null character
 * send_console_chars sends then adds the message into the message envelope, and then sends the envelope to i_crt_handler 
 * send_console_chars then sends a signal to i_crt_handler who outputs to the console
 * After tranmission is complete, the same envelope is returned to invoking process with message_type "display_ack" as confirmation
 * Inovking process does not block! So if the CRT is busy, will return EXIT_ERROR and invoking process must loop to ensure transmission is complete
 * Returns EXIT_SUCCESS if successful, EXIT_ERROR otherwise (eg. if message not terminated with null char or transmission fails */
int RTX::K_send_console_chars(MsgEnv* msgEnv)
{
	int ret = EXIT_ERROR;
		
	if(msgEnv != NULL) //error check
	{
		while(_semSend);
		_semSend = true;
		
		msgEnv->setOriginPid( getCurrentPid() );
		
		msgEnv->setMsgType(msgEnv->TO_CRT);

		if(K_send_message(PROC_CRT, msgEnv) != EXIT_ERROR)
		{
			kill(getpid(), SIGUSR2); //send signal to i_crt_handler who will handle transmitting the message	  

			if( (msgEnv = getMessage(MsgEnv::DISPLAY_ACK,this)) == NULL)
				if( (msgEnv = getMessage(MsgEnv::DISPLAY_FAIL,this)) == NULL)
					msgEnv = getMessage(MsgEnv::BUFFER_OVERFLOW,this);

			if(assure(msgEnv != NULL,"iProc_CRT did not return env",__FILE__,__LINE__,__func__,false) &&
				 msgEnv->getMsgType() == msgEnv->DISPLAY_ACK)	
					ret = EXIT_SUCCESS;
		}
		else
			msgEnv->setMsgType(msgEnv->DISPLAY_FAIL);
		
		if(msgEnv != NULL)
			K_send_message(getCurrentPid(), msgEnv);
	}
	return ret;
}

/* Invoking process provides a message envelope (previously allocated)
 * A message is sent to the invoking process using the envelope provided once the end of line character is received or the buffer is filled. 
 * Message type is "console_input"; message contains characters received
 * End of keyboard string is indicated by null character
 * Returns EXIT_SUCCESS if successful, EXIT_ERROR otherwise (i.e. no characters waiting) */
int RTX::K_get_console_chars(MsgEnv* msg_envelope)
{
	int res = EXIT_ERROR;
	if(msg_envelope != NULL)
	{
		atomic(true);
		int invoker = msg_envelope->getOriginPid();
		res = K_send_message(PROC_KB, msg_envelope);
		if(res == EXIT_SUCCESS)
		{
			msg_envelope = K_receive_message(); //message will be sent by iprocess
			string message = msg_envelope->getMsgData();
			
			msg_envelope->setMsgType(msg_envelope->CONSOLE_INPUT);	
			res = K_send_message(invoker, msg_envelope);
		}
		else
			debugMsg("RTX:333 Failed to send envelope!\n");

		atomic(false);
	}
	return res;
}

//calls getTraces, a function of the MsgTrace class
int RTX::K_get_trace_buffers(MsgEnv* msg_envelope)
{
	int ret = EXIT_ERROR;
	if(msg_envelope != NULL)
	{
		atomic(true);
		//call MsgTrace function to format trace buffers into table
		ret = _msgTrace->getTraces(msg_envelope); 
		atomic(false);
	}
	return ret;
}

/*
	NULL PROCESS_COUNT
	
	I do nothing!
*/
extern RTX* gRTX;
void RTX::null_proc() {
	while(true)
	{
		assure(gRTX != NULL, "gRTX NULL pointer",__FILE__,__LINE__,__func__,true);
		gRTX->K_release_processor();
	}
}

//Starts the first process executing on the CPU
void RTX::start_execution() 
{ 
	if(assure(!_started,"Attempting to start RTX multiple times",__FILE__,__LINE__,__func__,false))
	{
		_started = true;
		_scheduler->start(); 
	}
}
