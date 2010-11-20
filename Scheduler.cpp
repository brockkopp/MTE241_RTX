#include "Scheduler.h"
/*
Constructor

arguments: 
	currentProcess will be the first process to execute
	readyProcs: A queue of PCB pointers that point to all the procs that will start on the ready queue
	
*/

Scheduler::Scheduler(Queue* readyProcs)
{
	_readyProcs = new PQ(4);;
	_blockedEnv = new Queue( Queue::PROCCONBLOCK  );
	_blockedMsgRecieve = new Queue( Queue::PROCCONBLOCK  );
	
	//Add all readyProcs to the ready queue.
	for(int i=0; i < readyProcs->get_length(); i++) {
		PCB* temp = static_cast<PCB*>(readyProcs->dequeue_PCB());
		_readyProcs->pq_enqueue( temp, temp->get_priority() );
	}

	/*
	Somehow start the first proc.... Must set currentProc
	*/
}

Scheduler::~Scheduler() {
	delete _readyProcs;
	delete _blockedEnv;
	delete _blockedMsgRecieve;
}

///*
//Yields the CPU to the next available process, if there is one waiting.
//*/
void Scheduler::release_processor( ) { 
	//Save the context of the currently executing proc.
	//Does some crazy context save shinanigans need to be done here???
	_currentProcess->save_context();

	//Put currentProcess on the ready queue.
	_readyProcs->pq_enqueue( _currentProcess, _currentProcess->get_priority() );
	
	//Allow next process to start executing.
	//Note that if there is nothing waiting,
	//Then the single existing proc will be
	//put back on the CPU. Therefore, this
	//edgecase is covered.
	_currentProcess = _readyProcs->pq_dequeue();
	
	//Restore this proc's context
	_currentProcess->save_context();
} 

/* Will change the priority of the target proc.

arguments: 
	target: the PCB/proc whose priority you would wish to change.
	
returnValues: 
	-1 if PCB does not exist in any queue. 1 on success.
*/
int Scheduler::change_priority( PCB * target, int newPriority ) 
{ 
//	int oldPri = target->get_priority();

	//Case 1: PCB is on ready queue
	
	//If the target exists in the ready queue...
	if ( _readyProcs->pq_pluck( target ) ) { 
		
		//Change priority
		target->set_priority( newPriority );
		
		//Re-enqueue the PCB
		_readyProcs->pq_enqueue( target, target->get_priority() );
		
		return 1;
	}
	//Case 2a: PCB is on blockedEnv queue
	
	else if ( _blockedEnv->select( target ) ){
		//Remove PCB from queue
		 _blockedEnv->pluck( target );
		
		//Change priority
		target->set_priority( newPriority );
		
		//Re-enqueue the PCB
		 _blockedEnv->enqueue( target );
		
		return 1;
	}
	
	//Case 2b: PCB is on blockedMsg queue
	else if ( _blockedMsgRecieve->select( target ) ){
		//Remove PCB from queue
		 _blockedMsgRecieve->pluck( target );
		
		//Change priority
		target->set_priority( newPriority );
		
		//Re-enqueue the PCB
		 _blockedMsgRecieve->enqueue( target );
		 
		 return 1;
	}
	//Case 3: PCB is executing
	else if ( _currentProcess == target ){
		//Save context
		_currentProcess->save_context();
		
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
	PCB* nextProc =  _readyProcs->pq_dequeue();
	
	context_switch( nextProc );
	
	return -2;
}

/*
Actually switch a process off the CPU for the given process.

arguments:
	nextProc: the process to put onto the CPU
*/
int Scheduler::context_switch( PCB * nextProc ) 
{
	//Switch out _currentProcessfor nextProc.
	PCB* oldProc = _currentProcess;
	_currentProcess = nextProc;

	//Perform context_save shinanigans. See page in Sample Kernel Design
	//doc to see the suggested code that this is based on.
	int save_return = oldProc->save_context();
	
	//Restore context of next_proc iff setjmp is not returning from
	//a long_jmp
	if (save_return == 0) {
		_currentProcess->restore_context();
	}
	
	return 0;
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

return -2;
}


/*

arguments: 
	reason: g1 - blocked on envelope
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
		target->set_state( BLOCKED_ENV );
		return_value = _blockedEnv->enqueue( target );
			
	}	
	else if (reason == BLOCKED_MSG_RECIEVE){
		target->set_state( BLOCKED_MSG_RECIEVE );
		return_value = _blockedMsgRecieve->enqueue( target );
	}

	//Put the next available process on the ready queue
	if ( return_value == 0 )
		return process_switch();

	else
		return return_value;
}

/*
Moves process from a blocked queue ack onto the ready queue
and adjusts its status.


return values:
	0 - sucess
	1 - Process was (inFakt!) not blocked

*/
int Scheduler::unblock_process( PCB * target )
{
	//If process is blocked on msg recieve
	if (target->get_state() == BLOCKED_MSG_RECIEVE){
		_blockedMsgRecieve->pluck(target);	
		target->set_state( READY );

		//Re-enqueue on ready queue
		return _readyProcs->pq_enqueue( target , target->get_priority());
	}
	//If process is blocked on envelope
	else if (target->get_state() == BLOCKED_ENV) {
		_blockedEnv->pluck(target);
		target->set_state( READY );

		return _readyProcs->pq_enqueue( target , target->get_priority());
	}
	else //Process was not blocked in the first place...
		return 1;
}


/*
Return values: //Will return the state constant value depending on which type of blocked it is.
	0: 
	1: If proc is blocked on enveloper
	2: If proc is blocked on message recieve

*/
int Scheduler::is_blocked( PCB * target ) 
{
	return target->get_state();
}

PCB* Scheduler::get_blocked_on_env()
{
	return _blockedEnv->dequeue_PCB();
}

PCB* Scheduler::get_current_process() {
	return _currentProcess;
}
