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


/*
 
int Scheduler::change_priority( PCB * target, new_priority ) { return -2; }   
int Scheduler::process_switch( ) {return -2;}

int Scheduler::add_ready_process( PCB * target ) { return -2;}
int Scheduler::block_process (PCB * target, string reason ) {return -2; }
int Scheduler::unblock_process( PCB * target ) {return -2; }
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
int Scheduler::context_switch( PCB * next_proc ) {return -2;}
*/
int context_save( ) { return -2; }

