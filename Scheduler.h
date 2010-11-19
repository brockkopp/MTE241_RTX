#ifndef H_SCHEDULER
#define H_SCHEDULER


#include <string>
#include "debug.h"
#include "lib/PCB.h"
#include "lib/PQ.h"
#include "lib/Queue.h"

class Scheduler {
    public:

		Scheduler(Queue* readyProcs);	//Constructor!
		~Scheduler();
		
			void release_processor( );   
			int change_priority( PCB * target, int newPriority );
			int process_switch( );

			//Place new process on ready queue
      int add_ready_process( PCB * target );
      
//     	static const int BLOCKED_ENV = 1;
//			static const int BLOCKED_MSG_WAIT = 2;
			int block_process (PCB * target, int reason );
		int unblock_process( PCB * target );
			
		//Returns if a process is currently blocked on envelope
		int is_blocked( PCB * target );
		
		PCB* get_current_process();

    private:


    
      int context_switch( PCB * next_proc );
//      int context_save( );

			//Members
      PCB * _currentProcess; // Executing state

			PQ* _readyProcs; // Ready to execute state
      Queue* _blockedEnv; // Blocked on resource state
      Queue* _blockedMsgRecieve;
};


#endif
