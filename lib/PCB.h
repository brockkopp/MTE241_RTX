#ifndef H_PCB
#define H_PCB

#include <stdlib.h>
#include <setjmp.h>
#include "../debug.h"
#include "../RTX.h"
#include "PcbInfo.h"
#include "../tools.h"

class Mailbox;
class RTX;
class MsgEnv;

//Process state constant declarations.
#define READY 							0
#define BLOCKED_ENV 				1
#define BLOCKED_MSG_RECIEVE	2
#define SLEEPING 						3
#define EXECUTING 					4

class Queue;

class PCB 
{
	public:			
		/*~*~* Member functions ~*~*~*~*/
		PCB( PcbInfo* tableEntry );
		~PCB();				

		int getId( );
		string getName( );
		int getPriority( );
		int setPriority( int pri );
		int getProcessType( );
		int getState( );
		int setState( int state );
		string getStateName( );

		int incAtomicCount( );
		int decAtomicCount( );
		int getAtomicCount( );
		
		int saveContext();
		void restoreContext();

		//Mailbox modifiers
		MsgEnv* retrieveMail( );
		MsgEnv* retrieveMail( int msgType );
		MsgEnv* retrieveAck ( );
		bool addMail( MsgEnv* message );
		int checkMail( ); //returns number of messages in mailbox

	private:

		void initContext(int stackSize);

		/*~*~*~*~*~* Members *~*~*~*~*~*/
		int _atomicCount; 
		void (*_fPtr)(); 
		int _id;
		string _name;
		int _priority;   
		int _processType;
		char* _stack;
		int _state;
		Queue* _mailbox;
		jmp_buf _localJmpBuf;
};
#endif
