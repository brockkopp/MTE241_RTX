#include "debug.h"
#include "SignalHandler.h"
#include "Scheduler.h"
#include "CCI.h"
#include "lib/PCB.h"
#include "lib/PcbInfo.h"
#include "lib/MsgEnv.h"

//NOTE:Project Constants defined in "libs.h"

//RTX Global Constants
#define PROCESS_COUNT 	7
#define STACK_SIZE 	2034

#define PROCESS_I	1	
#define PROCESS_USER	2
#define PROCESS_K	3

class RTX
{
	public:
		RTX(PcbInfo* initTable[], SignalHandler* sigHandler);
		~RTX();
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

		PCB* pcbList[PROCESS_COUNT];
		Scheduler* scheduler;
		SignalHandler* signalHandler;
		CCI* cci;
};

//primitives

