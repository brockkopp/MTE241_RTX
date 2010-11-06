/*This class provides the Queue functionality*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string>

/* Type Definitions
 * --------------- */
typedef void* itemType;

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
		std::string _queueType;
		
	public:
	 Queue( std::string qtype );
	 ~Queue();
	 
	 int enqueue( itemType value ); //I'm not adding an insert function. If you want something on the list, enqueue it!
	 itemType* dequeue( ); 
	 
	 int isEmpty();
	 int contains( itemType value );
	 itemType* select( itemType value );
	 itemType* pluck ( itemType value );
	 int replace( itemType currValue, itemType newValue );
	 int get_length();
	 std::string get_queueType();
};
