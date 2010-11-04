#include "RTX.h"

RTX::RTX(PcbInfo* initTable[], SignalHandler* sigHandler)
{
	debugMsg("RTX Initializing...",0,1);
	signalHandler = sigHandler;
	cci = new CCI();

	for(int i=0; i < PROCESS_COUNT; i++)
	{
		pcbList[i] = new PCB(initTable[i]);
		debugMsg("\tProcess Created\n");	
	}
	debugMsg("RTX Init Done",0,1);
}

RTX::~RTX()
{
	delete cci;


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

