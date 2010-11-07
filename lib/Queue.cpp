/*This class provides the Queue functionality*/
#include "Queue.h"

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 *~*~*~*~*~*~*	 PRIVATE FUNCTIONS   *~*~*~*~*~*~*~*~*
 *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
int Queue::determineCastType()
{
	std::string qtype = _queueType;
	if(qtype == "int" || qtype == "INT")
		return INT;
	
	else if(qtype == "string" || qtype == "STRING" || qtype == "String")
		return STRING;
	
	else if(qtype == "MsgEnv" || qtype == "msgenv" || qtype == "MSGENV" || qtype == "Msg_Env" || qtype == "msg_env" || qtype == "MSG_ENV")
		return MSG_ENV;
	
	else if(qtype == "PCB" || qtype == "pcb")
		return PROCCONBLOCK;
	
	else
		return UNK_TYPE;
}

//Dequeues an object from the front of the queue.
//Returns a pointer to the dequeued object.
//Returns NULL if there is no node to dequeue.
itemType Queue::dequeue_gen()
{
	QueueNode* currentNode = _rear;
	
	//Special case: length = 0
	if (isEmpty()) //return null 
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
	return currentNode->item;	
}

//Remove the specified value from the queue and return a pointer to it
//Return NULL if the value is not found in the queue
itemType Queue::pluck_gen( itemType value )
{
	QueueNode* pluckee = _rear;
	QueueNode* prePluckee = _rear;
	int scanPos = _length - 1;
	while(pluckee != NULL)
	{
		if( pluckee->item == value ) //remove pluckee from queue
		{
			if(_length == 1) //special case
			{
				_rear = NULL;
				_front = NULL;
			}
			else //_length >= 2
			{
				if(scanPos == _length - 1) //plucking tail
					_rear = pluckee->link;
					
				else if(scanPos == 0) //plucking head
					_front = prePluckee;
					
				prePluckee->link = pluckee->link; //cut pluckee out
			}
			_length--;
			break;
		}
		prePluckee = pluckee;
		pluckee = pluckee->link;
		scanPos--;
	}
	return pluckee; //if value wasn't in queue, pluckee is NULL
}

// Find the specified value in the queue and return a pointer to it
//Return NULL if the value is not found in the queue
itemType Queue::select_gen( itemType value )
{		
	QueueNode* Temp = _rear;
	int scanPos = _length - 1;
	while(Temp != NULL)
	{
		if( Temp->item == value )
			return Temp->item;
		Temp = Temp->link;
		scanPos--;
	}
	return NULL;
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 *~*~*~*~*~*~*	  PUBLIC FUNCTIONS   *~*~*~*~*~*~*~*~*
 *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/

/*~*~*~*~*~*~* CONSTRUCTOR/DESTRUCTOR *~*~*~*~*~*~*~*~*/
Queue::Queue(std::string qtype)
{
	_front = NULL;
	_rear = NULL;	
	_length = 0;
	_queueType = qtype;
	_typeCastType = determineCastType();
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

/*~*~*~*~*~*~*	  ENQUEUE   *~*~*~*~*~*~*~*~*/
//Adds a node to the end of the queue containing the value parameter
//Returns true is enqueue is successful, returns false if unsuccessful (i.e. failed to allocate memory)
//NOTE: Calling funct
bool Queue::enqueue( itemType value )
{
	QueueNode* Temp;
	try{Temp = new QueueNode();}
	catch(char* str) { return false; } //if memory allocation for the pointer fails

	//perform required type-casting
	if(_typeCastType == INT)
		Temp->item = (int*)value;
	else if(_typeCastType == STRING)
		Temp->item = (std::string*)value;
	else if(_typeCastType == PROCCONBLOCK)
		Temp->item = (PCB*)value;	
	else if(_typeCastType == MSG_ENV)
		Temp->item = (MsgEnv*)value;

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
 return true;
}

/*~*~*~*~*~*~*	  DEQUEUE OVERLOADS   *~*~*~*~*~*~*~*~*/
itemType Queue::dequeue_itemType()
{
	return dequeue_gen();
}

int* Queue::dequeue_int()
{
	return (int*)(dequeue_gen());
}

std::string* Queue::dequeue_string()
{
	return (std::string*)(dequeue_gen());
}

MsgEnv* Queue::dequeue_MsgEnv()
{
	return (MsgEnv*)(dequeue_gen());
}

PCB* Queue::dequeue_PCB()
{
	return (PCB*)(dequeue_gen());
}

/*~*~*~*~*~*~*	  CONTAINS   *~*~*~*~*~*~*~*~*/
//Return true if the value is contained in the queue; return false otherwise
bool Queue::contains( itemType value )
{
	QueueNode* Temp = _rear;
	int position = _length - 1;
	while(Temp != NULL)
	{
		if(Temp->item == value)
			return true;
		Temp = Temp->link;
		position--;
	}

	return false;
}

/*~*~*~*~*~*~*	  GETTERS   *~*~*~*~*~*~*~*~*/
int Queue::get_length()
{
	return _length;
}

//Returns the queueType of the queue. If not defined(""), calling function MUST PERFORM CAST TYPING when a itemType* is being returned
std::string Queue::get_queueType()
{
	return _queueType;
}

//return true if queue is empty, return false otherwise
bool Queue::isEmpty()
{
	if(_front == NULL)
		return true;
		
	return false;
}

/*~*~*~*~*~*~*	  PLUCK OVERLOADS   *~*~*~*~*~*~*~*~*/
itemType Queue::pluck(itemType value)
{
	return pluck_gen(value);
}

int* Queue::pluck(int* value)
{
	return (int*)pluck_gen(value);
}

std::string* Queue::pluck(std::string* value)
{
	return (std::string*)pluck_gen(value);
}

MsgEnv* Queue::pluck(MsgEnv* value)
{
	return (MsgEnv*)pluck_gen(value);
}

PCB* Queue::pluck(PCB* value)
{
	return (PCB*)pluck_gen(value);
}

/*~*~*~*~*~*~*	  REPLACE   *~*~*~*~*~*~*~*~*/
//Find the currValue object in the queue and replace its item with newValue
//Return true if currValue does not exist in the queue; return false otherwise (if change is successful)
bool Queue::replace( itemType currValue, itemType newValue )
{
	itemType toChange = select(currValue);
	if(toChange == NULL)
		return false;
	
	toChange = newValue;
	return true;
}

/*~*~*~*~*~*~*	  SELECT OVERLOADS   *~*~*~*~*~*~*~*~*/
itemType Queue::select(itemType value)
{
	return select_gen(value);
}

int* Queue::select(int* value)
{
	return (int*)select_gen(value);
}

std::string* Queue::select(std::string* value)
{
	return (std::string*)select_gen(value);
}

MsgEnv* Queue::select(MsgEnv* value)
{
	return (MsgEnv*)select_gen(value);
}

PCB* Queue::select(PCB* value)
{
	return (PCB*)select_gen(value);
}

