/*This class provides the Queue functionality*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../debug.h"

/* Type Definitions
 * --------------- */
typedef int itemType;

typedef struct QueueNodeTag
{
	itemType item;
	struct QueueNodeTag *link;
}QueueNode;

/* Class Definition
 * --------------- */
class Queue 
{
	private:
		QueueNode* _front;
		QueueNode* _rear;
		int _length;
		
	public:
	 Queue();
	 //need a destructor?
	 
	 void enqueue( itemType value ); //I'm not adding an insert function. If you want something on the list, enqueue it!
	 itemType* dequeue(); //I'm not adding a delete function. If you want something from the list, dequeue it!
	 
	 int isEmpty();
	 int contains( itemType value );
	 itemType* select( unsigned int position );
	 int replace( unsigned int position, itemType value );
	 int get_length();
};
