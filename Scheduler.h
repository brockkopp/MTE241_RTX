#include "rtx.h"
/*
Dependencies are not yet complete.

#include "lib/PCB.h"
#include "lib/PQ.h"
*/
using namespace std;

class Scheduler {
    public:
/*
			Scheduler::Scheduler(); //Constructor!
*/			
			int release_processor( );   
/*			int change_priority( PCB * target, new_priority );   */
			int process_switch( );
       

			//Place new process on ready queue
/*
      bool add_ready_process( PCB * target );
			bool block_process (PCB * target, string reason );
			bool unblock_process( PCB * target );
*/
			//Returns if a process is currently blocked on envelope
/*
			bool is_blocked_on_envelope( PCB * target );
*/
    private:
/* 
      int context_switch( PCB * next_proc );
*/
      int context_save( );

			//Members
/*			    
      PCB * current_process; // Executing state

			PQ ready_procs; // Ready to execute state
      PQ blocked_env_procs; // Blocked on resource state
      LL_List blocked_msg_recieve;
*/      
}
