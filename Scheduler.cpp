#include "Scheduler.h"
/*
Constructor

arguments: 
	currentProcess will be the first process to execute
	readyProcs: A queue of PCB pointers that point to all the procs that will start on the ready queue
	
*/

extern RTX* gRTX;

Scheduler::Scheduler(Queue* readyProcs)
{
	_readyProcs = new PQ( 4 );
	_blockedEnv = new Queue( Queue::PROCCONBLOCK  );
	_blockedMsgRecieve = new Queue( Queue::PROCCONBLOCK  );
	_cpuTrace = "CPU trace does not track iniz of procs\n";
	
	//Add all readyProcs to the ready queue.
	PCB* temp;
	while (!readyProcs->isEmpty())
	{
		temp = readyProcs->dequeue_PCB();
		_readyProcs->pq_enqueue( &temp, temp->getPriority() );
	}
	_started = false;
}

Scheduler::~Scheduler() {
	delete _readyProcs;
	delete _blockedEnv;
	delete _blockedMsgRecieve;
}

//This is called to kick off CPU execution. It puts the first process to execute
//onto the cpu

void Scheduler::start() 
{
	//set current process to highest priority process 
	gRTX->_currentProcess = _readyProcs->pq_dequeue();
	gRTX->_currentProcess->setState( EXECUTING );
	//Restore context 

	gRTX->_currentProcess->restoreContext();	
}

///*
//Yields the CPU to the next available process, if there is one waiting.
//*/
int Scheduler::release_processor( ) { 

	//Save the context of the currently executing proc.
	if (gRTX->getCurrentPcb()->saveContext() == 0 ) {
				
		//Put currentProcess on the ready queue.
		gRTX->getCurrentPcb()->setState( READY );
		PCB* temp = gRTX->getCurrentPcb();
		_readyProcs->pq_enqueue( ((PCB**)(&temp)), gRTX->getCurrentPcb()->getPriority() );

		return process_switch();
	}
	return EXIT_SUCCESS;
}

/*
Switches the currently executing process off the CPU and replaces it 
with the next available ready process.
*/
int Scheduler::process_switch( ) 
{
	context_switch( _readyProcs->pq_dequeue() );

	return EXIT_SUCCESS;
}

/*
Actually switch a process off the CPU for the given process.

arguments:
	nextProc: the process to put onto the CPU
*/
int Scheduler::context_switch( PCB * nextProc ) 
{
	if (gRTX->getCurrentPcb()->saveContext() == 0) {

		//Put the new pcb on the cpu
		gRTX->setCurrentPcb( nextProc );
		gRTX->getCurrentPcb()->setState( EXECUTING );
		
		//Put process on CPU trace (for debugging etc...) and restore its context.
		_cpuTrace += gRTX->getCurrentPcb()->getName() + "\n";
		gRTX->getCurrentPcb()->restoreContext();
	}
	return EXIT_SUCCESS;
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
	
	//Validate priority
	if ( target == NULL || newPriority > 3 || newPriority < 0)
		return EXIT_ERROR;
		
	else if(target->getProcessType() == PROCESS_N)
		return EXIT_ERROR;
		
	//Case 1: PCB is on ready queue
	
	//If the target exists in the ready queue...
	if ( _readyProcs->pq_pluck( &target ) ) { 
		/* _readyProcs is a PQ, therefore when we change priority we must pluck
			the target and restore the target.
		*/
		
		//Change priority
		target->setPriority( newPriority );
		
		//Re-enqueue the PCB
		_readyProcs->pq_enqueue( &target, target->getPriority() );
		
		return EXIT_SUCCESS;
	}
	//Case 2a: PCB is on blockedEnv queue
	
	else if ( _blockedEnv->select( &target ) ){
		//Change priority
		target->setPriority( newPriority );
		
		return EXIT_SUCCESS;
	}
	
	//Case 2b: PCB is on blockedMsg queue
	else if ( _blockedMsgRecieve->select( &target ) ){

		//Change priority
		target->setPriority( newPriority );
		 
		 return EXIT_SUCCESS;
	}
	//Case 3: PCB is executing
	else if ( gRTX->getCurrentPcb() == target ){

		//Remove from executing, put on ready queue
		add_ready_process( target );
		
		//Process switch
		process_switch();
		
		return EXIT_SUCCESS;
	}
	
	//Case 4: PCB does not exist anywhere
	else {
		return EXIT_ERROR;
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
	if (gRTX->getCurrentPcb() == target){
		return EXIT_ERROR; //Failure, process is on CPU
	}

	else {
		/* Check if the process is on any blocked queues, if so, pluck it from there */
		_blockedEnv->pluck( &target );
		_blockedMsgRecieve->pluck( &target );
		
		/* Set the process state to READY and add it to the ready Procs queue */
		target->setState( READY );
		_readyProcs->pq_pluck( &target ); // <--- Just in case it already exists in the queue. *inefficient, intend to changed later.
		_readyProcs->pq_enqueue( &target, target->getPriority() );
		return EXIT_SUCCESS;
	}
}


/*

Blocks the currently executing process.

arguments: 
	reason: 1 - blocked on envelope
					2 - blocked on message recieve
					
*/
int Scheduler::block_process (int reason) 
{
	int return_value = EXIT_SUCCESS; //assume success

	//Remove process from CPU
	if (gRTX->getCurrentPcb()->getProcessType() == PROCESS_I) {
		/* I_processes may not be blocked */
		return EXIT_ERROR;
	}
	
	PCB* target = gRTX->getCurrentPcb();
	//Put process on appropriate blocked queue
	//and set its status
	if (reason == BLOCKED_ENV)
	{
		target->setState( BLOCKED_ENV );
		return_value = _blockedEnv->enqueue( (void**)(&target) );
	}	
	else if (reason == BLOCKED_MSG_RECIEVE){
		target->setState( BLOCKED_MSG_RECIEVE );
		return_value = _blockedMsgRecieve->enqueue( (void**)(&target) );
	}
	else if (reason == SLEEPING){
		target->setState( SLEEPING );
		return_value = _blockedMsgRecieve->enqueue( (void**)(&target) );
	}

	//Put the next available process on the ready queue
	return process_switch();
}

/*
Moves process from a blocked queue onto the ready queue
and adjusts its status.


return values:
	sucess
	error - Process was (inFakt!) not blocked

*/
int Scheduler::unblock_process( PCB * target )
{
	//If process is blocked on msg recieve
	if (target->getState() == BLOCKED_MSG_RECIEVE || 
			target->getState() == SLEEPING) {
			//Remove process from the blocked queue
			_blockedMsgRecieve->pluck(&target);
			

			if (target) {
				target->setState( READY );
				//Re-enqueue on ready queue
				return _readyProcs->pq_enqueue( &target , target->getPriority());
			}
		}
	
	//If process is blocked on envelope
	else if (target->getState() == BLOCKED_ENV) {
		_blockedEnv->pluck(&target);
		
		if (target) {
			target->setState( READY );
			return _readyProcs->pq_enqueue( &target , target->getPriority());
		}
	}
	
	//Process was not blocked in the first place...
		return EXIT_ERROR;
}

/*
Unblock a procesee off th apropriate queue. Use the state constants as reasons
*/

int Scheduler::unblock_process( int reason )
{
	//If process is blocked on msg recieve
	if (reason == BLOCKED_MSG_RECIEVE) {
			//Remove process from the blocked queue
			PCB* target = _blockedMsgRecieve->dequeue_PCB();
			
			if (target) {
				target->setState( READY );
				//Re-enqueue on ready queue
				return _readyProcs->pq_enqueue( &target , target->getPriority());
			}
		}
	
	//If process is blocked on envelope
	else if (reason == BLOCKED_ENV) {
		PCB* target = _blockedEnv->dequeue_PCB();
		
		if (target) {
			target->setState( READY );
			return _readyProcs->pq_enqueue( &target , target->getPriority());
		}
	}
	//Process was not blocked in the first place...
	//Or the queue was empty.
	return EXIT_ERROR;
}


/*
Return values: //Will return the state constant value depending on which type of blocked it is.
See declarations in header of PCB class.

*/

int Scheduler::setProcessState(int pid, int state)
{
PCB* tmpPcb;
	gRTX->getPcb(pid,&tmpPcb);
	
	// Ensure that PCB exists. State validation is done in PCB set fxn.
	if(tmpPcb == NULL)
		return EXIT_ERROR;
	else if(tmpPcb->setState(state) == EXIT_SUCCESS)
	{
		if(state == EXECUTING)
			gRTX->_signalHandler->setSigMasked(tmpPcb->getAtomicCount() > 0);
		return EXIT_SUCCESS;		
	}
	else
		return EXIT_ERROR;
}

int Scheduler::is_blocked( PCB * target ) 
{
	
	if ( target->getState() == BLOCKED_ENV ||
			 target->getState() == BLOCKED_MSG_RECIEVE
			)
			return EXIT_SUCCESS;
			
	else 
			return EXIT_ERROR;
}

PCB* Scheduler::get_blocked_on_env()
{
	return _blockedEnv->dequeue_PCB();
}
