#ifndef H_QUEUE
#define H_QUEUE

/*This class provides the Queue functionality*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string>
#include "PCB.h"
#include "MsgEnv.h"

//These constants will be used for type-casting
#define UNK_TYPE 0
#define INT 1
#define STRING 2
#define MSG_ENV 3
#define PROCCONBLOCK 4

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 *~*~* Type Definitions *~*~*
 ~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
typedef void* itemType;

typedef struct QueueNodeTag
{
	itemType item;
	struct QueueNodeTag *link;
}QueueNode;

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 *~*~* Class Definition *~*~*
 ~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
class Queue 
{
	private:
		QueueNode* _front;
		QueueNode* _rear;
		int _length;
		std::string _queueType;
		int _typeCastType;
		
		int determineCastType();		
	  itemType 		dequeue_gen();  
	  itemType 		pluck_gen(itemType value);
	  itemType 		select_gen(itemType value);
		
	public:
	 Queue( std::string qtype );
	 ~Queue();
	 
	 int enqueue( itemType value ); 
	 
	 itemType 		dequeue_itemType();
	 int* 				dequeue_int();
	 std::string* dequeue_string();
	 MsgEnv* 			dequeue_MsgEnv();
	 PCB* 				dequeue_PCB();
	 
	 int 					contains( itemType value );	 
	 int 					get_length();
	 std::string 	get_queueType();
	 int 					isEmpty();	
	  
	 itemType 		pluck ( itemType value );
	 int* 				pluck ( int* value );
	 std::string* pluck ( std::string* value );
	 MsgEnv* 			pluck ( MsgEnv* value );
	 PCB* 				pluck ( PCB* value );
	 
	 int replace( itemType currValue, itemType newValue );
	 
	 itemType 		select( itemType value );
	 int* 				select( int* value );
	 std::string* select( std::string* value );
	 MsgEnv* 			select( MsgEnv* value );
	 PCB* 				select( PCB* value );
};

#endif
