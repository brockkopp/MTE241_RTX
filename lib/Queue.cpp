/***** TO DO: ******/
//IS ITEMTYPE AN INT? OR SHOULD IT BE A VOID POINTER!?!?!?

/*This class provides the Queue functionality*/
#include "Queue.h"

using namespace std;

//constructor
Queue::Queue()
{
	_front = NULL;
	_rear = NULL;	
	_length = 0;
}

//adds a node to the queue containing the value parameter
void Queue::enqueue( itemType value )
{
	QueueNode* Temp = (QueueNode*)malloc(sizeof(QueueNode));
	assure(Temp == NULL, "System storage is exhausted - failed to allocate memory for queue",  __FILE__, __LINE__, true);

	Temp->item = value;

	if(_front == NULL) //adding first node
	{ 
		Temp->link = NULL;
		_front = Temp;	
		_rear	= Temp;
	}
	
	else 
	{
		Temp->link = _rear; 
		_rear = Temp;
	}
	_length++;	
}

//Dequeues an object from the front of the queue.
//Returns a pointer to the dequeued object.
//Returns NULL if there is no node to dequeue.
itemType* Queue::dequeue()
{
	QueueNode* currentNode = _rear;
	
	//Special case: length = 0
	if ( _length == 0 ) {
		
		//Throw error
		return NULL;
	}
	
	//Deal with special case of length = 1
	else if ( _length == 1 ) {
		_front = NULL;
		_rear = NULL;
		_length = 0;
		
		return &currentNode->Item;
	} 
	
	//Normal case
	else {
		//Loop through queue until we reach the second last entry
		for( int i = 0; i < _length - 1; i++) {
			currentNode = currentNode->link;
		}
		//Now currentNode = second to last node 
	
		//Set head to equal the second to last node
		_front = currentNode;
	
		_length --;
		
		//Return old front of queue
		return &currentNode->Item;
	}	
}

//return 0 if queue is empty, return 1 otherwise
int Queue::isEmpty()
{
	if(_front == NULL)
		return 0;
		
	return 1;
}

//Return position index of the node if the queue contains the value parameter
//Return -1 if value is NOT contained in the queue
int Queue::contains( itemType value )
{
	QueueNode* Temp = _rear;
	int position = _length - 1;
	while(Temp != NULL)
	{
		if(Temp->item == value)
			return position;
		Temp = Temp->link;
		position--;
	}

	return -1;
}

//Go to the position indicated in the input parameter and return a pointer to the node's item
//Return NULL if position is invalid
itemType* Queue::select( unsigned int position )
{
	if(position >= _length) //invalid reference position
		return NULL;
		
	QueueNode* Temp = _rear;
	int scanPos = _length - 1;
	while(Temp != NULL)
	{
		if(scanPos == position)
			return &(Temp->item);
		Temp = Temp->link;
		scanPos--;
	}
	return NULL;
}

//Go to the position indicated and change its item to value parameter
//Return -1 if position is invalid; return 0 otherwise (if change is successful)
int Queue::replace( unsigned int position, itemType value)
{
	itemType* toChange = select(position);
	if(toChange == NULL)
		return -1;
	
	*toChange = value;
	return 0;
}

int Queue::get_length()
{
	return _length;
}
