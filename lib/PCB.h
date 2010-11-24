#ifndef H_PCB
#define H_PCB

#include "../debug.h"
#include <stdlib.h>
#include "../RTX.h"
//#include "Context.h"
#include "PcbInfo.h"
#include "Mailbox.h"
#include <setjmp.h>
#include "../tools.h"
//#include <lib/Jmper.h>

class Mailbox;
class RTX;

//Process state constant declarations.
#define READY 				0
#define BLOCKED_ENV 		1
#define BLOCKED_MSG_RECIEVE 2
#define SLEEPING 			3
#define EXECUTING 			4


class Queue;

class PCB 
{
	public:			
		/*~*~* Member functions ~*~*~*~*/
		PCB();
		void init(PcbInfo* tableEntry);
		PCB( PcbInfo* tableEntry ); 	//Constructor
		PCB( PcbInfo* tableEntry, jmp_buf* jmpBuf ); 	//Constructor
		~PCB();							//Destructor
		
		//Private Member Getters/Setters
		int getId( );
		string getName( );
		int getPriority( );
		int setPriority( int pri );
		int getProcessType( );
		int getState( );
		int setState( int state );
		string getStateName();

		int incAtomicCount();
		int decAtomicCount();
		
		int saveContext();
		void restoreContext();

		//Mailbox modifiers
		MsgEnv* retrieveMail( );
		MsgEnv* retrieveMail( int msgType );
		MsgEnv* retrieveAck ( );
		bool addMail( MsgEnv* message );
		int checkMail( ); //returns number of messages in mailbox

//	private:
		/*~*~*~*~*~* Members *~*~*~*~*~*/
		//Context* _context;     //Includes jmp_buf
		int _atomicCount; 
		void (*_fPtr)(); 
		
		void initContext(int stackSize);
		
		int _id; //Process id
		string _name;
		int _priority;   
		int _processType;
		char* _stack;
		int _state;
		Mailbox* _mailbox; //Message mailbox
		jmp_buf _localJmpBuf;
		//Context is a public member - WHY?
};
#endif
