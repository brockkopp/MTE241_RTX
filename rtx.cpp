#include "rtx.h"

int K_send_message(int dest_process_id, Msg_Env* msg_envelope)
{
	return -2;
}

Msg_Env* K_receive_message()
{
	return null;
}

Msg_Env* K_request_msg_env()
{
	return null;
}

int K_release_msg_env(Msg_Env* memory_block)
{
	return -2;
}

int K_release_processor()
{
	return -2;
}

int K_request_process_status(Msg_Env* memory_block)
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

int K_request_delay(int time_delay, int wakeup_code, Msg_Env* msg_envelope)
{
	return -2;
}

int K_send_console_chars(Msg_Env* msg_envelope)
{
	return -2;
}

int K_get_console_chars(Msg_Env* msg_envelope)
{
	return -2;
}

int K_get_trace_buffers(Msg_Env* msg_envelope)
{
	return -2;
}

