//#include "PCB.h"

//#define STATE -2 //define all possible states and declare during constructors

///*~*~*~*~*~*~* Constructors *~*~*~*~*~*~*~*/
//PCB::PCB(PcbInfo* info)
//{ 
//	_atomicCount = 0;
//	_fPtr = (void(*))info->address;
//	_id = info->processId;
//	_name = info->name;
//	_priority = info->priority;
//	_processType = info->processType;	
//	_stack = (char *)(malloc(info->stackSize));
//	assure(_stack != NULL, "Stack initialization failed", __FILE__, __LINE__, __func__, true);

//	_state = STATE;
//	
//	Queue q("MsgEnv");
//	_mailbox = q;
//	
//	context = new Context(_stack, info->stackSize);		//process will suspend here until enqueued by scheduler I think
//	
//	//Execution returns here after process is enqueued (in Context constructor).
//	//Begin execution	
//	//_fPtr();	
//}

//PCB::PCB ( int processType, int priority ) 
//{	
//	_atomicCount = 0;
//	//init fPtr <-- how to we do this?
//	//init id <-- how to we do this?
//	//init name <-- ?
//	_priority = priority;
//	_processType = processType;
//	//Init stack <-- how to we do this?
//	//init state <-- how to we do this?
//	
//	Queue q("MsgEnv");
//	_mailbox = q;
//	//Context context;
//}

///*~*~*~*~*~*~* Destructors *~*~*~*~*~*~*~*/
//PCB::~PCB()
//{
//	delete[] _fPtr;
//	delete[] _stack;
//	delete[] &(_mailbox);
//	delete[] context;
//}

///*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
// ~*~*~*~* Getters/Setters ~*~*~*~*~~*~*
// *~*~*~*~*~~*~*~*~*~~*~*~*~*~~*~*~*~*~*/
//		
//int get_atomicCount() { return _atomicCount; }
//void set_atomicCount( int atomCount ) {	_atomicCount = atomCount; }
//		
//void* get_fPtr() { return _fPtr; }
//void set_fPtr( void* fPtr ) {	_fPtr = fPtr; }
//		
//int get_id() { return _id; }
//void set_id( int id ) {	_id = id; }
//		
//string get_name() { return _name; }
//void set_name( string name ) {	_name = name; }

//int get_priority() { return _priority; }
//int set_priority( int pri )//SHOULD THIS RE-ENQUEUE PCB IF IN RPQ????
//{
//	//Check if priority level exists
//	if (pri < 0 || pri > 3) 
//		return EXIT_ERROR; //error!
//	
//	else 
//	{
//		_priority = pri;
//		return EXIT_SUCCESS;
//	}
//}
//		
//int get_processType() { return _processType; }
//void set_processType( int processType ) {	_processType = processType; }
//		
//char* get_stack() { return _stack; }
//void set_stack( char* stack ) {	_stack = stack; } 
//		
//int get_state() { return _state; }
//void set_state( int state ) {	_state = state; } 
//				
///*~*~*~*~*~*~* Mailbox Modifiers *~*~*~*~*~*~*~*/
////Dequeues oldest message in the mailbox (FIFO)
//MsgEnv* retrieve_mail( )
//{
//	return _mailbox.dequeue_MsgEnv();
//}

////Enqueue message onto mailbox queue
//bool add_mail( MsgEnv* message )
//{
//	return (_mailbox.enqueue(message));
//}

////Returns the number of messages in the mailbox
//int check_mail( ) 
//{ 
//	return (_mailbox.get_length());
//}
//		


