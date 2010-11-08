#include "PCB.h"

#define STATE -2 //define all possible states and declare during constructors

/*~*~*~*~*~*~* Constructors *~*~*~*~*~*~*~*/
PCB::PCB(PcbInfo* info)
{ 
	_atomicCount = 0;
	_fPtr = (void(*))info->address;
	_id = info->processId;
	_name = info->name;
	_priority = info->priority;
	_processType = info->processType;	
	_stack = (char *)(malloc(info->stackSize));
	assure(_stack != NULL, "Stack initialization failed", __FILE__, __LINE__, __func__, true);

	_state = STATE;
	
	Queue q(q.MSG_ENV);
	_mailbox = q;
	
	context = new Context(_stack, info->stackSize);		//process will suspend here until enqueued by scheduler I think
	
	//Execution returns here after process is enqueued (in Context constructor).
	//Begin execution	
	//_fPtr();	
}

PCB::PCB ( int processType, int priority ) 
{	
	_atomicCount = 0;
	//init fPtr <-- how to we do this?
	//init id <-- how to we do this?
	//init name <-- ?
	_priority = priority;
	_processType = processType;
	//Init stack <-- how to we do this?
	//init state <-- how to we do this?
	
	Queue q(q.MSG_ENV);
	_mailbox = q;
	//Context context;
}

/*~*~*~*~*~*~* Destructors *~*~*~*~*~*~*~*/
PCB::~PCB()
{
	free(_fPtr); //void*, can't use delete because it'll try to dereference
	delete[] _stack;
	delete[] &(_mailbox);
	delete[] context;
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 ~*~*~*~* Getters/Setters ~*~*~*~*~~*~*
 *~*~*~*~*~~*~*~*~*~~*~*~*~*~~*~*~*~*~*/
		
int PCB::get_atomicCount() { return _atomicCount; }
void PCB::set_atomicCount( int atomCount ) {	_atomicCount = atomCount; }
		
void* PCB::get_fPtr() { return _fPtr; }
void PCB::set_fPtr( void* fPtr ) {	_fPtr = fPtr; }
		
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
				
/*~*~*~*~*~*~* Mailbox Modifiers *~*~*~*~*~*~*~*/
//Dequeues oldest message in the mailbox (FIFO)
MsgEnv* PCB::retrieve_mail( )
{
	return _mailbox.dequeue_MsgEnv();
}

//Enqueue message onto mailbox queue
bool PCB::add_mail( MsgEnv* message )
{
	return (_mailbox.enqueue(message));
}

//Returns the number of messages in the mailbox
int PCB::check_mail( ) 
{ 
	return (_mailbox.get_length());
}
		


