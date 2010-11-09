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

	if(_scheduler != NULL && _scheduler->_currentProcess != NULL)
		*pcb = _scheduler->_currentProcess;
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
	return -2;
}

MsgEnv* RTX::K_receive_message()
{
	return NULL;
}

MsgEnv* RTX::K_request_msg_env()
{
	return NULL;
}

int RTX::K_release_msg_env(MsgEnv* memory_block)
{
	return -2;
}

int RTX::K_release_processor()
{
	return -2;
}

int RTX::K_request_process_status(MsgEnv* memory_block)
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
	return -2;
}

int RTX::K_send_console_chars(MsgEnv* msg_envelope)
{
	return -2;
}

int RTX::K_get_console_chars(MsgEnv* msg_envelope)
{
	return -2;
}

int RTX::K_get_trace_buffers(MsgEnv* msg_envelope)
{
	return -2;
}
