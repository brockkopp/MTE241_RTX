#ifndef H_PCB
#define H_PCB

#include "../debug.h"
#include <stdlib.h>
#include "Context.h"
#include "PcbInfo.h"
#include "Mailbox.h"
#include "MsgEnv.h"

class Mailbox;

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
		//Constructors
		PCB( PcbInfo* tableEntry ); 
		
		//Destructor
		~PCB();
		
		//Private Member Getters/Setters

		void setAtomicCount( int atomCount );
		int incAtomicCount();
		int decAtomicCount();
		int getAtomicCount();
		
//		void* get_fPtr( );
//		void set_fPtr( void* fPtr );
		
		int getId( );
		void setId( int id );
		
		string getName( );
		void setName( string name );

		int getPriority( );
		int setPriority( int pri );
		
//		Context* getContext();
		int saveContext();
		void restoreContext();
		
		int getProcessType( );
//		void setProcessType( int processType );
		
//		char* getStack( );
//		void setStack( char* stack ); //DO WE NEED THESE for the stack!??!
		
		int getState( );
		int setState( int state );
		string getStateName();
				
		//Mailbox modifiers
		MsgEnv* retrieveMail( );
		MsgEnv* retrieveMail( int msgType );
		MsgEnv* retrieveMail( string msgType );
		bool addMail( MsgEnv* message );
		int checkMail( ); //returns number of messages in mailbox
		
		//ONLY USED BY SEND_CONSOLE_CHARS!!!
//		Queue* copyMailbox();
//		void emptyMailbox();
//    	void setMailbox(Queue* q);		
    		
	
	//private:
		/*~*~*~*~*~* Members *~*~*~*~*~*/
		Context* _context;     //Includes jmp_buf
		int _atomicCount; 
		void (*_fPtr)(); 
		
		int _id; //Process id
		string _name;
		int _priority;   
		int _processType;
		char* _stack;
		int _state;
		Mailbox* _mailbox; //Message mailbox
		//Context is a public member - WHY?
};
#endif
