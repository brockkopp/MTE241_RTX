#ifndef H_QUEUE
#define H_QUEUE

/*This class provides the Queue functionality*/
#include <stdio.h>
#include <stdbool.h>
#include "MsgEnv.h"
#include "PCB.h"
#include "../debug.h"
#include "../tools.h"

class PCB;
class MsgEnv;

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 *~*~* Type Definitions *~*~*
 ~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
typedef void* itemType;

typedef struct QueueNodeTag
{
	itemType item;
	struct QueueNodeTag *link;
	int priority;				//Not used in FIFO queues
}QueueNode;

typedef struct 
{
	MsgEnv* 	address;
	int 			allocatorID;
	int 			receiverID;
}envTrack;

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 *~*~* Class Definition *~*~*
 ~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
class Queue 
{
	public:
		//Constants
		static const	int UNK_TYPE = 			0;
		static const	int INT = 					1;
		static const	int STRING = 				2;
		static const	int MSG_ENV = 			3;
		static const	int PROCCONBLOCK = 	4; //PCB type
		#if DEBUG_MODE
			static const 	int TRACKER =				5;
		#endif
	
		Queue( );
		Queue( int qtype ); //qtype should be one of the constants declared above
		~Queue();

		bool 						enqueue( itemType value ); 
		bool 						sortedEnqueue( MsgEnv* newMsg, int priority );

		itemType 				dequeue_itemType();
		int* 						dequeue_int();
		std::string* 		dequeue_string();
		MsgEnv* 				dequeue_MsgEnv();
		PCB*						dequeue_PCB();
		
		bool 						contains( itemType value );	
		MsgEnv*					get_front(); 
		int 						get_length();
		int 						get_queueType(); //constants declared in Queue class!
		void 						set_queueType( int type ); //may only be done if _queueType not already defined
		bool 						isEmpty();	

		itemType 				pluck ( itemType value );
		int* 						pluck ( int* value );
		std::string* 		pluck ( std::string* value );
		MsgEnv* 				pluck ( MsgEnv* value );
		PCB* 						pluck ( PCB* value );
		
		#if DEBUG_MODE 
			envTrack*				pluck_Track ( MsgEnv* value );
		#endif
		
		bool 						replace( itemType currValue, itemType newValue );

		itemType 				select( itemType value );
		int* 						select( int* value );
		std::string* 		select( std::string* value );
		MsgEnv* 				select( MsgEnv* value );
		PCB* 						select( PCB* value );

		void 						printIntQueue(); //for testing purposes
		#if DEBUG_MODE 
			void					printTracker(); 
		#endif		
		string 					toString();
		
	protected:
		QueueNode* _front;
		QueueNode* _rear;
		int _length;
		int _queueType;
			
	  itemType 		dequeue_gen();  
	  itemType 		pluck_gen(itemType value);
	  itemType 		select_gen(itemType value);
	  
	  friend class Mailbox;
};

#endif
