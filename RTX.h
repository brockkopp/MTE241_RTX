#ifndef H_RTX
#define H_RTX

#include <sys/types.h>
#include <signal.h>
#include "debug.h"
#include "SignalHandler.h"
#include "Scheduler.h"
#include "MsgServ.h"
#include "lib/PCB.h"
#include "lib/MsgEnv.h"
#include "MsgTrace.h"

class PcbInfo;
class SignalHandler;
class MsgServ;
class Scheduler;

//RTX Global Constants
#define PROCESS_COUNT 	7		//Total number of processes existing in the RTX
#define STACK_SIZE 		2048	//Stack size in bytes

//Constants used to denote process types
#define PROCESS_I		1	
#define PROCESS_USER	2
#define PROCESS_K		3

#define USER_PROC_A		4
#define USER_PROC_B		5
#define USER_PROC_C		6

class RTX
{
	public:
		RTX(PcbInfo* initTable[], SignalHandler* signalHandler);
		~RTX();
		int getPcb(int pid, PCB** pcb);
		int getCurrentPcb(PCB** pcb);
		int atomic(bool on);
		
		int setCurrentProcess(int pid);
		
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
	
	private:
		PCB*						_pcbList[PROCESS_COUNT];		//Should be private, prevent invalid pid
		Scheduler* 			_scheduler;
		SignalHandler* 	_signalHandler;
		MsgServ* 				_mailMan;
};

#endif
