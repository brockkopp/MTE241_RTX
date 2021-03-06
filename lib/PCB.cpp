#include "PCB.h"

/*~*~*~*~*~*~* Constructors *~*~*~*~*~*~*~*/
extern RTX* gRTX;

PCB::PCB(PcbInfo* info)
{ 
	_atomicCount = 0;
	_id = info->processId;
	_name = info->name;
	_priority = info->priority;
	_processType = info->processType;	
	_fPtr = info->address;
	_stack = (char *)(malloc(info->stackSize));
	assure(_stack != NULL, "Stack initialization failed", __FILE__, __LINE__, __func__, true);
	_state = READY;

	_mailbox = new Queue(Queue::MSG_ENV);
	
	jmp_buf tempBuf;
	
	if( setjmp(tempBuf) == 0 )
	{
		char* stkPtr = _stack + info->stackSize - 8;	
		__asm__("movl %0,%%esp" :"=m" (stkPtr));

		if( setjmp( _localJmpBuf ) == 0 )
			longjmp(tempBuf ,1 );
		else {//First time the PCB is put on CPU. Function runs here.
			gRTX->getCurrentPcb()->_fPtr();
		}	
	}
	
	_link = NULL;
}

/*~*~*~*~*~*~* Destructors *~*~*~*~*~*~*~*/
PCB::~PCB()
{
	free(_stack);
	delete _mailbox;
}

int PCB::saveContext() 
{
	return setjmp( gRTX->getCurrentPcb()->_localJmpBuf );
}

void PCB::restoreContext() 
{
	longjmp( gRTX->getCurrentPcb()->_localJmpBuf, 1);
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 ~*~*~*~* Getters/Setters ~*~*~*~*~~*~*
 *~*~*~*~*~~*~*~*~*~~*~*~*~*~~*~*~*~*~*/

int PCB::incAtomicCount()
{
	return ++_atomicCount;
}
int PCB::decAtomicCount()
{
	assure(_atomicCount > 0,"Atomic count attempting to go out of bounds",__FILE__,__LINE__,__func__,false);
	if(_atomicCount > 0)
		return --_atomicCount;
	else
		return _atomicCount;
}
int PCB::getAtomicCount()
{
	return _atomicCount;
}
int PCB::getId() 
{ 
	return _id; 
}
		
string PCB::getName() 
{ 
	return _name; 
}

int PCB::getPriority() 
{ 
	return _priority; 
}

int PCB::setPriority( int pri )		//Should only be called by scheduler (my re-enqueue in RPQ)
{
	//Check if priority level exists
	if (pri < 0 || pri > 3) 
		return EXIT_ERROR; //error!
	
	else 
	{
		_priority = pri;
		return EXIT_SUCCESS;
	}
}
		
int PCB::getProcessType() 
{ 
	return _processType; 
}

int PCB::getState() 
{ 
	return _state; 
}
int PCB::setState( int state ) 
{	
	int ret = EXIT_SUCCESS;
	if(0 <= state && state <= 4)
		_state = state; 
	else
		ret = EXIT_ERROR;
	return ret;
} 
string PCB::getStateName()
{
	string state = "Unknown State";
	switch(_state)
	{
		case EXECUTING: 						state = "EXECUTING"; break;	
		case READY: 								state = "READY"; break;
		case BLOCKED_ENV: 					state = "BLK-ENVELOPE"; break;
		case BLOCKED_MSG_RECIEVE: 	state = "BLK-RECIEVE"; break;
		case SLEEPING: 							state = "SLEEPING"; break;
	}
	return state;
}

/*~*~*~*~*~*~* Mailbox Modifiers *~*~*~*~*~*~*~*/
//Dequeues oldest message in the mailbox (FIFO)
MsgEnv* PCB::retrieveMail( )
{
	return _mailbox->dequeue_MsgEnv();
}

//Enqueue message onto mailbox queue
bool PCB::addMail( MsgEnv* message )
{
	return (_mailbox->enqueue((void**)(&message)));
}

//Returns the number of messages in the mailbox
int PCB::checkMail( ) 
{ 
	return (_mailbox->get_length());
}
