#ifndef H_SCHEDULER
#define H_SCHEDULER


#include <string>
#include "debug.h"
#include "lib/PCB.h"
#include "lib/PQ.h"
#include "lib/Queue.h"

//Testing
#include "RTX.h"
class RTX;

class Scheduler {
    public:

			Scheduler(Queue* readyProcs);	//Constructor!
			~Scheduler();
		
			void release_processor( );   
			int change_priority( PCB * target, int newPriority );
			int process_switch( );

			//Place new process on ready queue
		  int add_ready_process( PCB * target );
			int block_process (PCB * target, int reason );
			int unblock_process( PCB * target );
			

			//Returns if a process is currently blocked on envelope
			int is_blocked( PCB * target );
			PCB*  get_blocked_on_env();
			PCB* get_current_process();
			
			void start(); //Starts the scheduler by putting the first process on the CPU
		
			int setCurrentProcess(PCB* newProcess);


    private:

      int context_switch( PCB * next_proc );

			//Members
      PCB * _currentProcess; // Executing state
			PQ* _readyProcs; // Ready to execute state
      Queue* _blockedEnv; // Blocked on resource state
      Queue* _blockedMsgRecieve;
};


#endif
