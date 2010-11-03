#include "RTX.h"

int K_send_message(int dest_process_id, MsgEnv* msg_envelope)
{
	return -2;
}

MsgEnv* K_receive_message()
{
	return NULL;
}

MsgEnv* K_request_msg_env()
{
	return NULL;
}

int K_release_msg_env(MsgEnv* memory_block)
{
	return -2;
}

int K_release_processor()
{
	return -2;
}

int K_request_process_status(MsgEnv* memory_block)
{
	return -2;
}

int K_terminate()
{
	return -2;
}

int K_change_priority(int new_priority, int target_process_id)
{
	return -2;
}

int K_request_delay(int time_delay, int wakeup_code, MsgEnv* msg_envelope)
{
	return -2;
}

int K_send_console_chars(MsgEnv* msg_envelope)
{
	return -2;
}

int K_get_console_chars(MsgEnv* msg_envelope)
{
	return -2;
}

int K_get_trace_buffers(MsgEnv* msg_envelope)
{
	return -2;
}

