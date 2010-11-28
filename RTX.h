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
#include "lib/WallClock.h"

class PcbInfo;
class SignalHandler;
class MsgServ;
class Scheduler;
class Queue;
class MsgEnv;
class MsgTrace;
class WallClock;

//RTX Global Constants

#define PROCESS_COUNT 8			//Total number of processes existing in the RTX
#define MSG_COUNT			20
#define TICK_TIME			100000 	// = 100mSec

//Constants used to denote process types
#define PROCESS_I			0	
#define PROCESS_K			2
#define PROCESS_U			2
#define PROCESS_N			3

#define PROC_TIMING 	0
#define PROC_KB				1
#define PROC_CRT 			2
#define PROC_NULL	 		3
#define PROC_USER_A 	4
#define PROC_USER_B 	5
#define PROC_USER_C 	6
#define PROC_CCI	 		7

class RTX
{
	public:
		WallClock* 			wallClock;
		
		RTX(PcbInfo* initTable[], SignalHandler* signalHandler);
		~RTX();
		int getPcb(int pid, PCB** pcb);
		PCB* getCurrentPcb();
		int getCurrentPid();
//		MsgEnv* retrieveOutAcknowledgement();
//		MsgEnv* retrieveInAcknowledgement();
		int atomic(bool on);

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
		
		#if DEBUG_MODE
		void K_print_enveloper_tracker();
		#endif
		
		static void null_proc();
		
		void start_execution();//Starts execution of processes on the CPU

		Queue* waitingProcesses;
		int runTime;

//	protected:
		int setCurrentPcb(PCB* pcb);
		int setCurrentPcb(int pid);

//	private:

		PCB**					_pcbList;		//Should be private, prevent invalid pid
		PCB*					_currentProcess;		
		Scheduler* 			_scheduler;
		SignalHandler* 	_signalHandler;

		MsgTrace*			_msgTrace;
		MsgServ* 			_mailMan;
		bool					_started;
		bool					_semSend;
				
		int send_chars_to_screen(MsgEnv* msg_envelope);
		
		friend class SignalHandler;
		friend class Scheduler;
};

#endif
