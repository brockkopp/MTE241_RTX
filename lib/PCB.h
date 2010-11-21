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
#define SLEEPING 3									//added process state - Eric/////////////////////

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
		
		void* get_fPtr( );
		void set_fPtr( void* fPtr );
		
		int get_id( );
		void set_id( int id );
		
		string get_name( );
		void set_name( string name );

		int get_priority( );
		int set_priority( int pri );
		
		Context* get_context();
		int save_context();
		void restore_context();
		
		int get_processType( );
		void set_processType( int processType );
		
		char* get_stack( );
		void set_stack( char* stack ); //DO WE NEED THESE for the stack!??!
		
		int get_state( );
		void set_state( int state );
		string getStateName();
				
		//Mailbox modifiers
		MsgEnv* retrieve_mail( );
		bool add_mail( MsgEnv* message );
		int check_mail( ); //returns number of messages in mailbox
		
		//ONLY USED BY SEND_CONSOLE_CHARS!!!
		Queue* copy_mailbox();
		void empty_mailbox();
    void set_mailbox(Queue* q);			

	private:
		/*~*~*~*~*~* Members *~*~*~*~*~*/
		Context* _context;     //Includes jmp_buf
		int _atomicCount; 
		void* _fPtr;  
		int _id; //Process id
		string _name;
		int _priority;   
		int _processType;
		char* _stack;
		int _state;
		Queue* _mailbox; //Message mailbox
		//Context is a public member - WHY?
};
#endif
