#include "Scheduler.h"

/*
Constructor

arguments: 
	currentProcess will be the first process to execute
	readyProcs: A queue of PCB pointers that point to all the procs that will start on the ready queue
	
*/

//Scheduler::Scheduler(PCB* currentProcess, Queue readyProcs):
//_readyProcs(4), _blockedEnv( "int" ), _blockedMsgRecieve( "int" )
// {
//	_currentProcess = currentProcess;
//	//PQ _readyProcs( 4 );
//	
//	
//	//Add all readyProcs to the ready queue.
//	for(int i=0; i<readyProcs.get_length(); i++) {
//		PCB* temp = static_cast<PCB*>(readyProcs.dequeue());
//		_readyProcs.penqueue( temp, temp->get_priority() );
//	}

//}

///*
//Yields the CPU to the next available process, if there is one waiting.
//*/
//void Scheduler::release_processor( ) { 
//	//Save the context of the currently executing proc.
//	//Does some crazy context save shinanigans need to be done here???
//	_currentProcess->context->save();

//	//Put currentProcess on the ready queue.
//	_readyProcs.penqueue( _currentProcess );
//	
//	//Allow next process to start executing.
//	//Note that if there is nothing waiting,
//	//Then the single existing proc will be
//	//put back on the CPU. Therefore, this
//	//edgecase is covered.
//	_currentProcess = _readyProcs.pdequeue();
//	
//	//Restore this proc's context
//	_currentProcess.context.save();
//} 

///* Will change the priority of the target proc.

//arguments: 
//	target: the PCB/proc whose priority you would wish to change.
//*/
//int Scheduler::change_priority( PCB * target, newPriority ) 
//{ 
//	int oldPri = target->get_priority();

//	//Case 1: PCB is on ready queue
//	
//	//If the target exists in the ready queue...
//	if ( _readyProcs[oldPri].select( target ) ) { 
//		//Remove PCB from queue
//		_readyProcs[oldPri].pluck( target );
//		
//		//Change priority
//		target.set_priority( newPriority );
//		
//		//Re-enqueue the PCB
//		_readyProcs.penqueue( target, target->get_priority() );
//		
//		return 1;
//	}
//	//Case 2a: PCB is on blockedEnv queue
//	
//	else if ( _blockedEnvProcs.select( target ) ){
//		//Remove PCB from queue
//		 _blockedEnvProcs.pluck( target );
//		
//		//Change priority
//		target.set_priority( newPriority );
//		
//		//Re-enqueue the PCB
//		 _blockedEnvProcs.enqueue( target );
//		
//		return 1;
//	}
//	
//	//Case 2b: PCB is on blockedMsg queue
//	else if ( _blockedMsgRecieve.select( target ) ){
//		//Remove PCB from queue
//		 _blockedMsgRecieve.pluck( target );
//		
//		//Change priority
//		target.set_priority( newPriority );
//		
//		//Re-enqueue the PCB
//		 _blockedMsgRecieve.enqueue( target );
//		 
//		 return 1;
//	}
//	//Case 3: PCB is executing
//	else if ( _currentProcess == target ){
//		//Save context
//		_currentProcess.context.save();
//		
//		//Remove from executing, put on ready queue
//		add_ready_process( target );
//		
//		//Process switch
//		process_switch();
//		
//		return 1;
//	}
//	
//	//Case 4: PCB does not exist in any queue
//	else {
//		return -1;
//	}
//}   

///*
//Switches the currently executing process off the CPU and replaces it 
//with the next available ready process.
//*/
//int Scheduler::process_switch( ) {
//	PCB* nextProc =  _readyProcs.pdequeue();
//	
//	context_switch( nextProc );
//}

///*
//Actually switch a process off the CPU for the given process.

//arguments:
//	nextProc: the process to put onto the CPU
//*/
//int Scheduler::context_switch( PCB * nextProc ) 
//{
//	//Switch out _currentProcessfor nextProc.
//	oldProc = _currentProcess;
//	_currentProcess = nextProc;

//	//Perform context_save shinanigans. See page in Sample Kernel Design
//	//doc to see the suggested code that this is based on.
//	save_return = old_proc.context.save();
//	
//	//Restore context of next_proc iff setjmp is not returning from
//	//a long_jmp
//	if (save_return == 0) {
//		_currentProcess.context.restore();
//	}
//}

///*
//Will put a process onto the ready queue from anywhere (except if 
//it is already executing. Therefore the process is either new, 
//or blocked on one to the two blocker queues.

//arguments: target PCB to put on ready queue.
//*/
//int Scheduler::add_ready_process( PCB * target ) 
//{
//	if (_currentProcess == target){
//		return 0; //Failure, process is on CPU
//	}

////	if ( target->get_status() == )

//}


///*

//arguments: 
//	reason: 1 - blocked on envelope
//					2 - blocked on message recieve
//					
//*/
//int Scheduler::block_process (PCB * target, int reason ) 
//{
//	int return_value = 0; //assume success

//	//Remove process from CPU
//	
//	//Put process on appropriate blocked queue
//	//and set its status
//	if (reason == BLOCKED_ENV)
//	{
//		target.set_status( BLOCKED_ENV );
//		return_value = _blockedEnv.penqueue( target );
//			
//	}	
//	else if (reason == BLOCKED_MSG_WAIT){
//		target.set_status( BLOCKED_MSG_WAIT );
//		return_value = _blockedMsgRecieve;
//	}

//	//Put the next available process on the ready queue
//	if ( return_value == 0 )
//		return process_switch();

//	else
//		return return_value;
//}

///*
//Moves process from a blocked queue ack onto the ready queue
//and adjusts its status.


//return values:
//	0 - sucess
//	1 - Process was (inFakt!) not blocked

//*/
//int Scheduler::unblock_process( PCB * target )
//{
//	//If process is blocked on msg recieve
//	if (target->get_status() == BLOCKED_MSG){
//		_blockedMsgRecieve.dequeue();	
//		target->set_status( READY );

//		//Re-enqueue on ready queue
//		return _readyProcs.equeue( target );
//	}
//	//If process is blocked on envelope
//	else if (target->get_status() == BLOCKED_ENV) {
//		_blockedEnv.dequeue();
//		target->set_status( READY );

//		return _readyProcs.enqueue( target );
//	}
//	else //Process was not blocked in the firs place...
//		return 1;
//}


///*
//Return values: //Will return the state constant value depending on which type of blocked it is.
//	0: 
//	1: If proc is blocked on enveloper
//	2: If proc is blocked on message recieve

//*/
//int Scheduler::is_blocked( PCB * target ) 
//{
//	return target->get_state();
//}

//PCB* get_current_process() {
//	return _currentProcess;

//}
