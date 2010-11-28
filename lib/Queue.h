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
typedef void* QueueNode; //could be 

typedef struct envTrackTag
{
	MsgEnv* 	address;
	int 			allocatorID;
	int 			receiverID;
	struct envTrackTag* _link;
}envTrack;

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 *~*~* Class Definition *~*~*
 ~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
class Queue 
{
	public:
		//Constants
		static const	int UNK_TYPE = 			0;
		static const	int MSG_ENV = 			1;
		static const	int PROCCONBLOCK = 	2; //PCB type
		#if DEBUG_MODE
			static const 	int TRACKER =			3;
		#endif
	
		Queue( );
		Queue( int qtype ); //qtype should be one of the constants declared above
		~Queue();

		bool 						enqueue( QueueNode* value ); 
		bool 						sortedEnqueue( MsgEnv** newMsg);

		MsgEnv* 				dequeue_MsgEnv();
		PCB*						dequeue_PCB();
		
		bool 						contains( QueueNode* value );	
		MsgEnv*					get_front(); 
		int 						get_length();
		int 						get_queueType(); //constants declared in Queue class!
		void 						set_queueType( int type ); //may only be done if _queueType not already defined
		bool 						isEmpty();	

		MsgEnv* 				pluck ( MsgEnv** value );
		PCB* 						pluck ( PCB** value );
		
		QueueNode 			select( QueueNode* value );
		MsgEnv* 				select( MsgEnv** value );
		PCB* 						select( PCB** value );
		
		#if DEBUG_MODE 
			envTrack*				pluck_Track ( MsgEnv** value );
		#endif
		
		bool 						replace( QueueNode* currValue, QueueNode* newValue );
		string 					toString();

		#if DEBUG_MODE 
			void					printTracker(); 
		#endif		
		
	protected:
		QueueNode _front;
		QueueNode _rear;
		int _length;
		int _queueType;
				  
	  friend class Mailbox;
};

#endif
