#include "Scheduler.h"

/*
Constructor

arguments: 
	currentProcess will be the first process to execute
	readyProcs[]: An array of PCB pointers that point to all the procs that will start on the ready queue
	
*/

Scheduler::Scheduler(PCB* currentProcess, PCB* readyProcs[]) {
	_currentProcess = currentProcess;
	
	//Add all readyProcs to the ready queue.
	int sizeOfReadyProcs = sizeof( readyProcs ) / sizeof( readyProcs[0] );
	
	for(int i=0; i<sizeOfReadyProcs; i++) {
		_readyProcs.penqueue( readyProcs[i] );
	}

}

/*
Yields the CPU to the next available process, if there is one waiting.
*/
void Scheduler::release_processor( ) { 
	//Save the context of the currently executing proc.
	//Does some crazy context save shinanigans need to be done here???
	_currentProcess->context->save();

	//Put currentProcess on the ready queue.
	_readyProcs.penqueue( _currentProcess );
	
	//Allow next process to start executing.
	//Note that if there is nothing waiting,
	//Then the single existing proc will be
	//put back on the CPU. Therefore, this
	//edgecase is covered.
	_currentProcess = _readyProcs.pdequeue();
	
	//Restore this proc's context
	_currentProcess.context.save();
} 

/* Will change the priority of the target proc.

arguments: 
	target: the PCB/proc whose priority you would wish to change.
*/
int Scheduler::change_priority( PCB * target, newPriority ) 
{ 
	int oldPri = target->get_priority();

	//Case 1: PCB is on ready queue
	
	//If the target exists in the ready queue...
	if ( _readyProcs[oldPri].select( target ) ) { 
		//Remove PCB from queue
		_readyProcs[oldPri].pluck( target );
		
		//Change priority
		target.set_priority( newPriority );
		
		//Re-enqueue the PCB
		_readyProcs.penqueue( target, target->get_priority() );
		
		return 1;
	}
	//Case 2a: PCB is on blockedEnv queue
	
	else if ( _blockedEnvProcs.select( target ) ){
		//Remove PCB from queue
		 _blockedEnvProcs.pluck( target );
		
		//Change priority
		target.set_priority( newPriority );
		
		//Re-enqueue the PCB
		 _blockedEnvProcs.enqueue( target );
		
		return 1;
	}
	
	//Case 2b: PCB is on blockedMsg queue
	else if ( _blockedMsgRecieve.select( target ) ){
		//Remove PCB from queue
		 _blockedMsgRecieve.pluck( target );
		
		//Change priority
		target.set_priority( newPriority );
		
		//Re-enqueue the PCB
		 _blockedMsgRecieve.enqueue( target );
		 
		 return 1;
	}
	//Case 3: PCB is executing
	else if ( _currentProcess == target ){
		//Save context
		_currentProcess.context.save();
		
		//Remove from executing, put on ready queue
		add_ready_process( target );
		
		//Process switch
		process_switch();
		
		return 1;
	}
	
	//Case 4: PCB does not exist in any queue
	else {
		return -1;
	}
}   

/*
Switches the currently executing process off the CPU and replaces it 
with the next available ready process.
*/
int Scheduler::process_switch( ) {
	PCB* nextProc =  _readyProcs.pdequeue();
	
	context_switch( nextProc );
}

/*
Actually switch a process off the CPU for the given process.

arguments:
	nextProc: the process to put onto the CPU
*/
int Scheduler::context_switch( PCB * nextProc ) 
{
	//Switch out _currentProcessfor nextProc.
	oldProc = _currentProcess;
	_currentProcess = nextProc;

	//Perform context_save shinanigans. See page in Sample Kernel Design
	//doc to see the suggested code that this is based on.
	save_return = old_proc.context.save();
	
	//Restore context of next_proc iff setjmp is not returning from
	//a long_jmp
	if (save_return == 0) {
		_currentProcess.context.restore();
	}
}

/*
Will put a process onto the ready queue from anywhere (except if 
it is already executing. Therefore the process is either new, 
or blocked on one to the two blocker queues.

arguments: target PCB to put on ready queue.
*/
int Scheduler::add_ready_process( PCB * target ) 
{
	if (_currentProcess == target){
		return 0; //Failure, process is on CPU
	}

//	if ( target->get_status() == )

}


/*

arguments: 
	reason: 1 - blocked on envelope
					2 - blocked on message recieve
					
*/
int Scheduler::block_process (PCB * target, int reason ) 
{
	int return_value = 0; //assume success

	//Remove process from CPU
	
	//Put process on appropriate blocked queue
	//and set its status
	if (reason == BLOCKED_ENV)
	{
		target.set_status( BLOCKED_ENV );
		return_value = _blockedEnv.penqueue( target );
			
	}	
	else if (reason == BLOCKED_MSG_WAIT){
		target.set_status( BLOCKED_MSG_WAIT );
		return_value = _blockedMsgRecieve;
	}

	//Put the next available process on the ready queue
	if ( return_value == 0 )
		return process_switch();

	else
		return return_value;
}
/*int Scheduler::unblock_process( PCB * target ) {return -2; }
*/
//Returns if a process is currently blocked on envelope
/*
int Scheduler::is_blocked_on_envelope( PCB * target ) {return -2; }
*/
/*
PCB * Scheduler::current_process {return NULL;} // Executing state

PQ * Scheduler::ready_procs() {return NULL;} // Ready to execute state
PQ * Scheduler::blocked_env_procs() {return NULL;} // Blocked on resource state
List Scheduler::blocked_msg_recieve() {return NULL;}
*/
/*

*/
int context_save( ) { return -2; }

