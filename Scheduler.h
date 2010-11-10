/* Q * Q * Q * Q * Q * Q * Q * Q * Q * Q * Q * Q * 
Q * Q *  QUESTION: are we keeping currentProcess a private member? or should we make it public?
Q * Q * Q * Q * Q * Q * Q * Q * Q * Q * Q * Q * Q * Q */

#include <string>
#include "debug.h"
#include "lib/PCB.h"
#include "lib/PQ.h"

class Scheduler {
    public:

		Scheduler(PCB* currentProcess, Queue readyProcs); //Constructor!
			
//			void release_processor( );   
//			int change_priority( PCB * target, int newPriority );
//			int process_switch( );
//       

//			//Place new process on ready queue
//      bool add_ready_process( PCB * target );
//			bool block_process (PCB * target, string reason ); //i think this string should be an int - Eric
//			bool unblock_process( PCB * target );
//			
//			//Returns if a process is currently blocked on envelope
//			bool is_blocked( PCB * target );

//    private:
//      int context_switch( PCB * next_proc );
//      int context_save( );

			//Members
      PCB * _currentProcess; // Executing state

			PQ _readyProcs; // Ready to execute state
      Queue _blockedEnv; // Blocked on resource state
      Queue _blockedMsgRecieve;
};
