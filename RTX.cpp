#include "RTX.h"

RTX::RTX(PcbInfo* initTable[], SignalHandler* sigHandler)
{
	debugMsg("RTX Initializing...",0,1);
	//Inititalize RTX members, each cascades to its own constructor which performs memory allocation
	signalHandler = sigHandler;
	cci = new CCI();

	//Initialize each PCB from init table
	for(int i=0; i < PROCESS_COUNT; i++)
	{
		pcbList[i] = new PCB(initTable[i]);
		debugMsg("\tProcess Created: ");
		debugMsg(pcbList[i]->name,0,1);	
	}
	debugMsg("RTX Init Done",0,1);
}

RTX::~RTX()
{
	//Free resources held by each RTX member, allocated in the RTX constructor
	delete cci;

	//Signal handling no long exists
	delete signalHandler;
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
