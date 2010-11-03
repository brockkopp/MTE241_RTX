#include "debug.h"
#include "lib/PcbInfo.h"
#include "lib/MsgEnv.h"
#include "lib/PCB.h"

//NOTE:Project Constants defined in "libs.h"

//RTX Global Constants
#define PROCESS_COUNT 	6
#define STACK_SIZE 	2034

class RTX
{
	public:
		PCB* pcbList[];
		RTX(PcbInfo* initTable[]);
		int K_send_message(int dest_process_id, MsgEnv* msg_envelope);
		MsgEnv* K_receive_message();
		MsgEnv* K_request_msg_env();
		int K_release_msg_env(MsgEnv* memory_block);
		int K_release_processor();
		int K_request_process_status(MsgEnv* memory_block);
		int K_terminate();
		int K_change_priority(int new_priority, int target_process_id);
		int K_request_delay(int time_delay, int wakeup_code, MsgEnv* msg_envelope);
		int K_send_console_chars(MsgEnv* msg_envelope);
		int K_get_console_chars(MsgEnv* msg_envelope);
		int K_get_trace_buffers(MsgEnv* msg_envelope);
};

//primitives

