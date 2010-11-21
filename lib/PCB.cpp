#include "PCB.h"

#define STATE -2 //define all possible states and declare during constructors

/*~*~*~*~*~*~* Constructors *~*~*~*~*~*~*~*/
void nothing3(){cout << "HHH\n\n";}

PCB::PCB(PcbInfo* info)
{ 
	_atomicCount = 0;
	//_fPtr = (void(*))info->address;
	_id = info->processId;
	_name = info->name;
	_priority = info->priority;
	_processType = info->processType;	
	_stack = (char *)(malloc(info->stackSize));
	assure(_stack != NULL, "Stack initialization failed", __FILE__, __LINE__, __func__, true);

	_state = STATE;
	
	_mailbox = new Queue(Queue::MSG_ENV);
	int i;
	//_fPtr = &(nothing3);
	_fPtr = info->address;
	

//	_context = new Context(_stack, info->stackSize, tmp_fxn);	
//	_context->init(_stack, info->stackSize, tmp_fxn);
	_context = new Context(_stack, info->stackSize, _fPtr);	
	_context->init(_stack, info->stackSize, _fPtr);
}



/*~*~*~*~*~*~* Destructors *~*~*~*~*~*~*~*/
PCB::~PCB()
{

	//free(_fPtr); //void*, can't use delete because it'll try to dereference
	free(_stack);
	delete _mailbox;
	delete[] _context;
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
		
int PCB::get_id() { return _id; }
void PCB::set_id( int id ) {	_id = id; }
		
string PCB::get_name() { return _name; }
void PCB::set_name( string name ) {	_name = name; }

int PCB::get_priority() { return _priority; }
int PCB::set_priority( int pri )//SHOULD THIS RE-ENQUEUE PCB IF IN RPQ????
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
		
int PCB::get_processType() { return _processType; }
void PCB::set_processType( int processType ) {	_processType = processType; }
		
char* PCB::get_stack() { return _stack; }
void PCB::set_stack( char* stack ) {	_stack = stack; } 
		
int PCB::get_state() { return _state; }
void PCB::set_state( int state ) {	_state = state; } 
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

Context* PCB::get_context() { return _context; }
int PCB::save_context() { return _context->save(); }

void PCB::restore_context() { _context->restore(); }
				
/*~*~*~*~*~*~* Mailbox Modifiers *~*~*~*~*~*~*~*/
//Dequeues oldest message in the mailbox (FIFO)
MsgEnv* PCB::retrieve_mail( )
{
	return _mailbox->dequeue_MsgEnv();
}

//Enqueue message onto mailbox queue
bool PCB::add_mail( MsgEnv* message )
{
	return (_mailbox->enqueue(message));
}

//Returns the number of messages in the mailbox
int PCB::check_mail( ) 
{ 
	return (_mailbox->get_length());

}

Queue* PCB::copy_mailbox()
{
	return _mailbox;
}

void PCB::empty_mailbox()
{
	delete (_mailbox);
	return;
}

void PCB::set_mailbox(Queue* q)
{
	_mailbox = q;
	return;
}
		


