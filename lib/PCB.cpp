#include "PCB.h"

/*~*~*~*~*~*~* Constructors *~*~*~*~*~*~*~*/
extern RTX* gRTX;
extern PCB* gCurrentProcess;

PCB::PCB() {};

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

	//_localJmpBuf = (jmp_buf*)malloc(sizeof(jmp_buf));

	_mailbox = new Mailbox();
	
	initContext(info->stackSize);
	//_context = new Context(_stack, info->stackSize, _fPtr);	
}

void PCB::init(PcbInfo* info)
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

	//_localJmpBuf = (jmp_buf*)malloc(sizeof(jmp_buf));

	_mailbox = new Mailbox();
	
	initContext(info->stackSize);
	//_context = new Context(_stack, info->stackSize, _fPtr);	
}

/*~*~*~*~*~*~* Destructors *~*~*~*~*~*~*~*/
PCB::~PCB()
{
	free(_stack);
	delete _mailbox;
	//delete _context;
}

void PCB::initContext(int stackSize)
{
	//SEE rtxInitialization on UW-ACE
	jmp_buf tempBuf;
	//Init the function pointer.

	if( setjmp(tempBuf) == 0 )
	{
	
		
			char* stkPtr = _stack + stackSize - 8;	
			__asm__("movl %0,%%esp" :"=m" (stkPtr));
		//_set_sp(stackPtr + stackSize);
		if( setjmp( _localJmpBuf ) == 0 )
		{
//			cout << "savPtr: " << &_fPtr <<  endl;
	cout << "Init, jmp_buf" << _localJmpBuf << "\n";	
			
//			cout << "ini: " << _localJmpBuf << endl;
			longjmp(tempBuf ,1 );
		}
		else //First time the PCB is put on CPU. Function runs here.
		{
//			PCB* tmp;
//			gRTX->getCurrentPcb(&tmp);
//			cout << "run:\t" << _localJmpBuf << "   " << /*tmp->getName() <<*/ endl;
//			cout << "runPtr:\t" << endl;
//			cout << "runPcb:\t" << tmp->getName() << endl;
cout << "Restored, global current proc" << gCurrentProcess << "\n";
cout << "Restored, global current proc's _fPtr" << &gCurrentProcess->_fPtr << "\n";
cout << "Restored, jmp_buf" << _localJmpBuf << "\n";
cout << "Abut to start fptr....\n";
//			_fPtr();
gCurrentProcess->_fPtr();

		}
	}
}

int PCB::saveContext() 
{
	PCB* tmp;
	gRTX->getCurrentPcb(&tmp);
//	debugMsg("%Context: about to SAVE context: " + tmp->getName() + "%\n");
	//return gSaveContext(_localJmpBuf);
	
//	return setjmp( _localJmpBuf );

debugMsg("%Context: about to SAVE context: " + tmp->getName() + "%\n");
int ret = setjmp( gCurrentProcess->_localJmpBuf );
debugMsg("%Context: returned from SAVE context: " + tmp->getName() + " with return value: "+ intToStr(ret)  +"%\n");
return ret;
}

void PCB::restoreContext() 
{
	PCB* tmp;
//	gRTX->getCurrentPcb(&tmp);
//	debugMsg("%Context: about to RESTORE context: " + tmp->getName() + "%\n");
//	cout << "res: " << _localJmpBuf << endl;
	//gRestoreContext(_localJmpBuf);
	cout << "About to jump, jmp_buf" << _localJmpBuf << "\n";

//gCurrentProcess = _readyProcs->pq_dequeue();
//gCurrentProcess->setState( EXECUTING );
//gCurrentProcess->restoreContext();	
	gRTX->getCurrentPcb(&tmp);
	gCurrentProcess = tmp;
	longjmp( _localJmpBuf, 1);
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
		case EXECUTING: 			state = "EXECUTING"; break;	
		case READY: 				state = "READY"; break;
		case BLOCKED_ENV: 			state = "BLK-ENV"; break;
		case BLOCKED_MSG_RECIEVE: 	state = "BLK-REC"; break;
		case SLEEPING: 				state = "SLEEPING"; break;
	}
	return state;
}

/*~*~*~*~*~*~* Mailbox Modifiers *~*~*~*~*~*~*~*/
//Dequeues oldest message in the mailbox (FIFO)
MsgEnv* PCB::retrieveMail( )
{
	return _mailbox->getMail();
}

MsgEnv* PCB::retrieveMail( int msgType )
{
	return _mailbox->getMail( msgType );
}

MsgEnv* PCB::retrieveAck()
{
	MsgEnv* ret;
	//Attempt to retrieve a display acknowledgement
	ret = _mailbox->getMail( MsgEnv::DISPLAY_ACK );
	//If no acknowledgements, search for display failure
	if(ret == NULL)
		ret = _mailbox->getMail( MsgEnv::DISPLAY_FAIL );
	if(ret == NULL)
		ret = _mailbox->getMail( MsgEnv::BUFFER_OVERFLOW );
	//return message (or NULL)
	return ret;
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
