#include "PCB.h"

/*~*~*~*~*~*~* Constructors *~*~*~*~*~*~*~*/

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
	
	_mailbox = new Mailbox();
	_context = new Context(_stack, info->stackSize, _fPtr);	
}

/*~*~*~*~*~*~* Destructors *~*~*~*~*~*~*~*/
PCB::~PCB()
{

	//free(_fPtr); //void*, can't use delete because it'll try to dereference
	free(_stack);
	delete _mailbox;
	delete _context;
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 ~*~*~*~* Getters/Setters ~*~*~*~*~~*~*
 *~*~*~*~*~~*~*~*~*~~*~*~*~*~~*~*~*~*~*/
		
//int PCB::get_atomicCount() { return _atomicCount; }
//void PCB::set_atomicCount( int atomCount ) {	_atomicCount = atomCount; }
int PCB::incAtomicCount()
{
	return ++_atomicCount;
}
int PCB::decAtomicCount()
{
	assure(_atomicCount > 0,"Atomic count out of bounds",__FILE__,__LINE__,__func__,true);
	return --_atomicCount;
}
int PCB::getAtomicCount()
{
	return _atomicCount;
}
		
//void* PCB::get_fPtr() { return _fPtr; }
//void PCB::set_fPtr( void* fPtr ) {	_fPtr = &fPtr; }
		
int PCB::getId() 
{ 
	return _id; 
}
void PCB::setId( int id ) 
{	
	_id = id; 
}
		
string PCB::getName() 
{ 
	return _name; 
}
void PCB::setName( string name ) 
{	
	_name = name; 
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
//void PCB::setProcessType( int processType ) {	_processType = processType; }
		
//char* PCB::getStack() { return _stack; }
//void PCB::setStack( char* stack ) {	_stack = stack; } 
		
int PCB::getState() 
{ 
	return _state; 
}
int PCB::setState( int state ) 
{	
	int ret = EXIT_SUCCESS;
	if(0 >= state && state <= 4)
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
		case READY: 				state = "Ready"; break;
		case BLOCKED_ENV: 			state = "Blocked-Env"; break;
		case BLOCKED_MSG_RECIEVE: 	state = "Blocked-Rx"; break;
		case SLEEPING: 				state = "Asleep"; break;
	}
	return state;
}

//Context* PCB::getContext() { return _context; }
int PCB::saveContext() 
{ 
	return _context->save(); 
}
void PCB::restoreContext() 
{ 
	_context->restore(); 
}

/*~*~*~*~*~*~* Mailbox Modifiers *~*~*~*~*~*~*~*/
//Dequeues oldest message in the mailbox (FIFO)
MsgEnv* PCB::retrieveMail( )
{
	return _mailbox->getMail();
}

//Enqueue message onto mailbox queue
bool PCB::addMail( MsgEnv* message )
{
	return (_mailbox->deliverMail(message));
}

//Returns the number of messages in the mailbox
int PCB::checkMail( ) 
{ 
	return (_mailbox->getSize());
}

//Queue* PCB::copyMailbox()
//{
//	return _mailbox;
//}

//void PCB::emptyMailbox()
//{
//	delete (_mailbox);
//	return;
//}
//void PCB::setMailbox(Queue* q)
//{
//	_mailbox = q;
//	return;
//}
