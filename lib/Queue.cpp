//TO DO 
//ENQUEUE: VERIFYING ENQUEUING RIGHT string?
//PLUCK
//use constants for int, string, pcb, etc.

/*This class provides the Queue functionality*/
#include "Queue.h"
/*
Please explain what values the qtype can be.
*/


//Constructors
Queue::Queue(std::string qtype)
{
	_front = NULL;
	_rear = NULL;	
	_length = 0;
	_queueType = qtype;
}

//Destructor: delete all memory used by the queue
//This destructor will automatically be called by the c++ delete function
Queue::~Queue()
{
 	if(!isEmpty()) //ensure queue is not already empty
 	{
 		QueueNode* currNode;
 		QueueNode* nextNode = _rear;
		while(nextNode != NULL)
		{
			currNode = nextNode;
			nextNode = currNode->link;
			delete[] currNode;
		}
 	}
}

//VERIFYING ENQUEUING RIGHT string?
//Adds a node to the queue containing the value parameter
//Returns 0 is enqueue is successful, returns 1 if unsuccessful (i.e. failed to allocate memory)
int Queue::enqueue( itemType value )
{
//	QueueNode* Temp;
//	try{Temp = new QueueNode();}
//	catch() { return 1; }

//	Temp->item = value;

//	if(_front == NULL) //adding first node
//	{ 
//		Temp->link = NULL;
//		_front = Temp;	
//		_rear	= Temp;
//	}
//	
//	else 
//	{
//		Temp->link = _rear; 
//		_rear = Temp;
//	}
//	_length++;
// return 0;
	return -2;
}

//Dequeues an object from the front of the queue.
//Returns a pointer to the dequeued object.
//Returns NULL if there is no node to dequeue.
itemType* Queue::dequeue()
{
	QueueNode* currentNode = _rear;
	
	//Special case: length = 0
	if (isEmpty()) //Throw error 
		return NULL;
	
	//Deal with special case of length = 1
	else if ( _length == 1 ) 
	{
		_front = NULL;
		_rear = NULL;
	} 
	
	//Normal case
	else 
	{
		//Loop through queue until we reach the second last entry
		for( int i = 0; i < _length - 2; i++) 
		{
			currentNode = currentNode->link;
		}
		//Now currentNode = second to last node 
	
		//Set head to equal the second to last node
		_front = currentNode;
	}
		_length--;
		
		//Return old front of queue
		return &(currentNode->item);	
}

//Return 0 if the value is contained in the queue; return 1 otherwise
int Queue::contains( itemType value )
{
	QueueNode* Temp = _rear;
	int position = _length - 1;
	while(Temp != NULL)
	{
		if(Temp->item == value)
			return 0;
		Temp = Temp->link;
		position--;
	}

	return 1;
}

// Find the specified value in the queue and return a pointer to it
//Return NULL if the value is not found in the queue
itemType* Queue::select( itemType value )
{		
	QueueNode* Temp = _rear;
	int scanPos = _length - 1;
	while(Temp != NULL)
	{
		if( Temp->item == value )
			return &(Temp->item);
		Temp = Temp->link;
		scanPos--;
	}
	return NULL;
}

//Remove the specified value from the queue and return a pointer to it
//Return NULL if the value is not found in the queue
itemType* Queue::pluck ( itemType value )
{
//if doesn't exist
	return NULL;
}

//Find the currValue object in the queue and replace its item with newValue
//Return 1 if currValue does not exist in the queue; return 0 otherwise (if change is successful)
int Queue::replace( itemType currValue, itemType newValue )
{
	itemType* toChange = select(currValue);
	if(toChange == NULL)
		return 1;
	
	*toChange = newValue;
	return 0;
}

int Queue::get_length()
{
	return _length;
}

//Returns the string of the queue. If empty string (""), calling function MUST PERFORM CAST TYPING when a itemType* is being returned
std::string Queue::get_queueType()
{
	return _queueType;
}

//return 0 if queue is empty, return 1 otherwise
int Queue::isEmpty()
{
	if(_front == NULL)
		return 0;
		
	return 1;
}
