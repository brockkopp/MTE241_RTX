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
	
	//Add all readyProcs to the ready queue.
	//int numProcs =	readyProcs->get_length();
	PCB* temp;
	while (!readyProcs->isEmpty())
	{
		temp = readyProcs->dequeue_PCB();
		_readyProcs->pq_enqueue( temp, temp->getPriority() );
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
void Scheduler::release_processor( ) { 

	//Save the context of the currently executing proc.
	//Do some crazy context save shinanigans need to be done here???
	if (gRTX->getCurrentPcb()->saveContext() == 0 ) {
				
		//Put currentProcess on the ready queue.
//		gRTX->getCurrentPcb()->setState( READY );
//		_readyProcs->pq_enqueue( gRTX->getCurrentPcb(), gRTX->getCurrentPcb()->getPriority() );
						
		//Allow next process to start executing.
		//Note that if there is nothing waiting,
		//Then the single existing proc will be
		//put back on the CPU. Therefore, this
		//edgecase is covered.

//		process_switch();


process_switch();
/*************************88
	TO BE REPLACED BY PROCESS_SWITCH*/
//		gRTX->setCurrentPcb(_readyProcs->pq_dequeue());

//		gRTX->getCurrentPcb()->setState( EXECUTING );
//	
//		//Restore this proc's context		
//		gRTX->getCurrentPcb()->restoreContext();
/***************************************************************/
	}
}

/*
Switches the currently executing process off the CPU and replaces it 
with the next available ready process.
*/
int Scheduler::process_switch( ) {
	context_switch( _readyProcs->pq_dequeue());

	return 1;
}

/*
Actually switch a process off the CPU for the given process.

arguments:
	nextProc: the process to put onto the CPU
*/
int Scheduler::context_switch( PCB * nextProc ) 
{
//	cout << "NEXT PROC: " << nextProc->getName();
//	cin.get();
//	//Switch out _currentProcessfor nextProc.
//	PCB* oldProc = gRTX->getCurrentPcb();
//	gRTX->setCurrentPcb(nextProc);
//	gRTX->getCurrentPcb()->setState( EXECUTING );
//	
//	//Put old proc onto ready queue
//	oldProc->setState( READY );
//	_readyProcs->pq_enqueue( oldProc, oldProc->getPriority() );
//	
//	//Perform context_save shinanigans. See page in Sample Kernel Design
//	//doc to see the suggested code that this is based on.
//	
//	//Restore context of next_proc iff setjmp is not returning from
//	//a long_jmp
//	if (oldProc->saveContext()) {
//		gRTX->getCurrentPcb()->restoreContext();
//	}
//	
//	return 1;
	if (gRTX->getCurrentPcb()->saveContext() == 0) {
		//Put current proc onto ready queue
		gRTX->getCurrentPcb()->setState( READY );
		_readyProcs->pq_enqueue( gRTX->getCurrentPcb(), gRTX->getCurrentPcb()->getPriority());
	
		//Put the new pcb on the cpu
		gRTX->setCurrentPcb( nextProc );
		gRTX->getCurrentPcb()->setState( EXECUTING );
		gRTX->getCurrentPcb()->restoreContext();
	}
	return 1;
}

//void context_switch(jmp_buf * previous,
//return_code = setjmp(*previous);
//if (return_code == 0)
//{
//longjmp(*next,1);





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
		target->setPriority( newPriority );
		
		//Re-enqueue the PCB
		_readyProcs->pq_enqueue( target, target->getPriority() );
		
		return 1;
	}
	//Case 2a: PCB is on blockedEnv queue
	
	else if ( _blockedEnv->select( target ) ){
		//Remove PCB from queue
		 _blockedEnv->pluck( target );
		
		//Change priority
		target->setPriority( newPriority );
		
		//Re-enqueue the PCB
		 _blockedEnv->enqueue( target );
		
		return 1;
	}
	
	//Case 2b: PCB is on blockedMsg queue
	else if ( _blockedMsgRecieve->select( target ) ){
		//Remove PCB from queue
		 _blockedMsgRecieve->pluck( target );
		
		//Change priority
		target->setPriority( newPriority );
		
		//Re-enqueue the PCB
		 _blockedMsgRecieve->enqueue( target );
		 
		 return 1;
	}
	//Case 3: PCB is executing
	else if ( gRTX->getCurrentPcb() == target ){
		//Save context
		gRTX->getCurrentPcb()->saveContext();
		
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
Will put a process onto the ready queue from anywhere (except if 
it is already executing. Therefore the process is either new, 
or blocked on one to the two blocker queues.

arguments: target PCB to put on ready queue.
*/
int Scheduler::add_ready_process( PCB * target ) 
{
	if (gRTX->getCurrentPcb() == target){
		return 0; //Failure, process is on CPU
	}

	else {
		/* Check if the process is on any blocked queues, if so, pluck it from there */
		_blockedEnv->pluck( target );
		_blockedMsgRecieve->pluck( target );
		
		/* Set the process state to READY and add it to the ready Procs queue */
		target->setState( READY );
		_readyProcs->pq_pluck( target ); // <--- Just in case it already exists in the queue. *inefficient, intend to changed later.
		_readyProcs->pq_enqueue( target, target->getPriority() );
		return 1;
	}
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
		target->setState( BLOCKED_ENV );
		return_value = _blockedEnv->enqueue( target );
			
	}	
	else if (reason == BLOCKED_MSG_RECIEVE){
		target->setState( BLOCKED_MSG_RECIEVE );
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
	if (target->getState() == BLOCKED_MSG_RECIEVE){
		_blockedMsgRecieve->pluck(target);	
		target->setState( READY );

		//Re-enqueue on ready queue
		return _readyProcs->pq_enqueue( target , target->getPriority());
	}
	//If process is blocked on envelope
	else if (target->getState() == BLOCKED_ENV) {
		_blockedEnv->pluck(target);
		target->setState( READY );

		return _readyProcs->pq_enqueue( target , target->getPriority());
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

int Scheduler::setProcessState(int pid, int state)
{
	PCB* tmpPcb;
	gRTX->getPcb(pid,&tmpPcb);
	if(tmpPcb == NULL)
		return EXIT_ERROR;
	else
		return tmpPcb->setState(state);
}

int Scheduler::is_blocked( PCB * target ) 
{
	return target->getState();
}

PCB* Scheduler::get_blocked_on_env()
{
	return _blockedEnv->dequeue_PCB();
}

//PCB* Scheduler::get_current_process() {
//	return gRTX->_currentProcess;
//}

//int Scheduler::setCurrentProcess(int pid)
//{
//	PCB* newProcess;
//	gRTX->getPcb(pid,&newProcess);
//	if(newProcess == NULL)
//		return EXIT_ERROR;
//	else
//		return setCurrentProcess(newProcess);
//}
//int Scheduler::setCurrentProcess(PCB* newProcess)
//{
//	if(newProcess == NULL)
//		return EXIT_ERROR;
//		
//	gRTX->_currentProcess = newProcess;
//	return EXIT_SUCCESS;
//}
