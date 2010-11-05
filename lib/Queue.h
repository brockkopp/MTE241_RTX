/*This class provides the Queue functionality*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Type Definitions
 * --------------- */
typedef void* itemType;

typedef struct QueueNodeTag
{
	itemType item;
	struct QueueNodeTag *link;
}QueueNode;

typedef string QueueType;

/* Class Definition
 * --------------- */
class Queue 
{
	private:
		QueueNode* _front;
		QueueNode* _rear;
		int _length;
		int _queueType
		
	public:
	 Queue( QueueType type );
	 ~Queue(); //TO IMPLEMENT
	 
	 int enqueue( itemType value ); //I'm not adding an insert function. If you want something on the list, enqueue it!
	 itemType* dequeue( ); //I'm not adding a delete function. If you want something from the list, dequeue it!
	 
	 int isEmpty();
	 int contains( itemType value );
	 itemType* select( itemType value );
	 itemType* pluck ( itemType value );
	 int replace( itemType currValue, itemType newValue );
	 int get_length();
	 string get_queueType();
};
