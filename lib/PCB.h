#ifndef H_PCB
#define H_PCB

#include "../debug.h"
#include <stdlib.h>
#include "Context.h"
#include "PcbInfo.h"
#include "Queue.h"
#include "MsgEnv.h"

//Process state constant declarations.
#define READY 0
#define BLOCKED_ENV 1
#define BLOCKED_MSG_RECIEVE 2

class Queue;

class PCB 
{
	public:	
		/*~*~*~*~*~* Members *~*~*~*~*~*/
		Context* context;     //Includes jmp_buf
		
		/*~*~* Member functions ~*~*~*~*/
		//Constructors
		PCB( PcbInfo* tableEntry );
		PCB( int processType, int priority ); 
		
		//Destructor
		~PCB();
		
		//Private Member Getters/Setters

		void setAtomicCount( int atomCount );

		int incAtomicCount();
		int decAtomicCount();
		int getAtomicCount();
		
		void* get_fPtr( );
		void set_fPtr( void* fPtr );
		
		int get_id( );
		void set_id( int id );
		
		string get_name( );
		void set_name( string name );

		int get_priority( );
		int set_priority( int pri );
		
		int get_processType( );
		void set_processType( int processType );
		
		char* get_stack( );
		void set_stack( char* stack ); //DO WE NEED THESE for the stack!??!
		
		int get_state( );
		void set_state( int state );
				
		//Mailbox modifiers
		MsgEnv* retrieve_mail( );
		bool add_mail( MsgEnv* message );
		int check_mail( ); //returns number of messages in mailbox
		

	private:
		/*~*~*~*~*~* Members *~*~*~*~*~*/
		int _atomicCount; 
		void* _fPtr;  
		int _id; //Process id
		string _name;
		int _priority;   
		int _processType;
		char* _stack;
		int _state;
		Queue _mailbox; //Message mailbox
		//Context is a public member - WHY?
};
#endif
