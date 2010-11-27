/*This class provides the Queue functionality*/
#include "Queue.h"
/*
Please explain what values the qtype can be.
*/


/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 *~*~*~*~*~*~*	 PRIVATE FUNCTIONS   *~*~*~*~*~*~*~*~*
 *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
//Dequeues an object from the front of the queue.
//Returns a pointer to the dequeued object.
//Returns NULL if there is no node to dequeue.
itemType Queue::dequeue_gen()
{
	void* retData = NULL;
	
	if (!isEmpty()) 
	{
		QueueNode* currentNode = _rear;

		//Deal with special case of length = 1
		if ( _rear == _front ) 
		{
			retData = _rear->item;
			delete _rear;
			_front = NULL;
			_rear = NULL;
		} 
		else 
		{
			//Loop through queue until we reach the second last entry
			for( int i = 0; i < _length - 2; i++) 
				currentNode = currentNode->link;

			_front = currentNode;			//Set _front to second last node (last node to be deleted)
			retData = _front->link->item;	//Save data of last node
			
			delete _front->link;			//Delete last node
			_front->link = NULL;
		}
	  	_length--;
	}
	//Return old front of queue
	return retData;	
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
			else
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
	if(pluckee == NULL)
		return NULL;
	return pluckee->item; //if value wasn't in queue, pluckee is NULL
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
Queue::Queue()
{
	_front = NULL;
	_rear = NULL;	
	_length = 0;
	_queueType = UNK_TYPE;
}

//Queuetypes are defined as constants in the header...
Queue::Queue(int qtype)
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
			delete currNode;
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
	Temp->priority = -1;
	
	//perform required type-casting
	if(_queueType == INT)
		Temp->item = (int*)value;
	else if(_queueType == STRING)
		Temp->item = (std::string*)value;
	else if(_queueType == PROCCONBLOCK)
		Temp->item = (PCB*)value;	
	else if(_queueType == MSG_ENV)
		Temp->item = (MsgEnv*)value;
	#if DEBUG_MODE
		else if(_queueType == TRACKER)
			Temp->item = (envTrack*)value;
	#endif

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


bool Queue::sortedEnqueue( MsgEnv* newMsg, int priority )
{
	if(_queueType == MSG_ENV)
	{
		QueueNode* tempNode = new QueueNode();
		tempNode->item = newMsg;
		tempNode->priority = priority;
		
		if(_front == NULL)										//Empty Queue
		{
			tempNode->link = NULL;
			_front = tempNode;	
			_rear  = tempNode;
		}
		else
		{
			QueueNode* curr = _rear;

			if(curr->link == NULL)								//Only 1 Node
			{
				if(curr->priority > tempNode->priority)
				{
					curr->link = tempNode;
					_front = tempNode;
				}
				else
				{
					tempNode->link = curr;
					_rear = tempNode;
				}
			}
			else if(tempNode->priority < _front->priority)		//Last Item
			{
				_front->link = tempNode;
				_front = tempNode;
			}
			else
			{
				bool done = false;
				QueueNode* prev = NULL;			//higher
				do
				{
					if(tempNode->priority > curr->priority)	//Add left
					{
						tempNode->link = curr;
						if(_rear == curr)
							_rear = tempNode;
						else
							prev->link = tempNode;
						done = true;				
					}
					prev = curr;
					curr = curr->link;
				}
				while(!done && curr != NULL);					//2+ items
			}
		}
		_length++;
		return true;
	}

	return false;

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
MsgEnv* Queue::get_front()
{
	if(_queueType == Queue::MSG_ENV)
		return (MsgEnv*)_front;
	else
		return NULL;
}

int Queue::get_length()
{
	return _length;
}

//Returns the queueType of the queue. If not defined(ie UNK_TYPE 0), calling function MUST PERFORM CAST TYPING when a itemType* is being returned
int Queue::get_queueType()
{
	return _queueType;
}

//Sets the queueType of the queue. May only be done if the _queueType was not previously defined
void Queue::set_queueType( int type )
{
	if(_queueType == UNK_TYPE)
		_queueType = type;
	return;
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

#if DEBUG_MODE
envTrack* Queue::pluck_Track(MsgEnv* value)
{
	QueueNode* pluckee = _rear;
	QueueNode* prePluckee = _rear;
	int scanPos = _length - 1;
	while(pluckee != NULL)
	{
		if( ((envTrack*)(pluckee->item))->address == value ) //remove pluckee from queue
		{
			if(_length == 1) //special case
			{
				_rear = NULL;
				_front = NULL;
			}
			else
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
	if(pluckee == NULL)
		return NULL;
	return (envTrack*)pluckee->item; 
}
#endif

/*~*~*~*~*~*~*	  REPLACE   *~*~*~*~*~*~*~*~*/
//Find the currValue object in the queue and replace its item with newValue
//Return true if currValue does not exist in the queue; return false otherwise (if change is successful)
bool Queue::replace( itemType currValue, itemType newValue )
{
	QueueNode* toChange = _rear;
	int scanPos = _length - 1;
	while(toChange != NULL)
	{
		if( toChange->item == currValue )
			break;
		toChange = toChange->link;
		scanPos--;
	}

	if(toChange == NULL)
		return 0;	

	if(_queueType == INT)
		toChange->item = (int*)newValue;
	else if(_queueType == STRING)
		toChange->item = (std::string*)newValue;
	else if(_queueType == PROCCONBLOCK)
		toChange->item = (PCB*)newValue;	
	else if(_queueType == MSG_ENV)
		toChange->item = (MsgEnv*)newValue;
	else
		toChange->item = newValue;
	return 1;
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

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 *~*~*~*~*~*~*	    FOR TESTING      *~*~*~*~*~*~*~*~*
 *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
 
string Queue::toString()
{
	QueueNode* Temp = _rear;
	int position = _length - 1;
	
	string output = "{ ";
	
	while(Temp != NULL)
	{
		switch(_queueType)
		{
			case INT:
				output += "<-[" + intToStr(position) + ": " + intToStr(*(int*)(Temp->item)) + "]";
				break;
			case STRING:
				output += "<-[" + intToStr(position) + ": " +*(string*)(Temp->item) + "]";
				break;
			case MSG_ENV:
				output += "<-[" + intToStr(position) + ": " + intToStr(Temp->priority) + "]";
				break;
			case PROCCONBLOCK:
				output += "<-[" + intToStr(position) + ": " + ((PCB*)(Temp->item))->getName() + "]";
				break;
		}
		Temp = Temp->link;
		position--;
	}
	return output + " }";
}

void Queue::printIntQueue() //assume _typeCastType == INT
{
	if(_queueType == INT)
		debugMsg("Queue: " + toString());
}

#if DEBUG_MODE
void Queue::printTracker() 
{
	QueueNode* Temp = _rear;
	int position = _length - 1;
	
	cout<<" {\n";
	
	while(Temp != NULL)
	{
		cout<<"\t<-["
				<<intToStr(position) 
				<<": A_ID "			<<((envTrack*)(Temp->item))->allocatorID
				<<": D_ID "			<<((envTrack*)(Temp->item))->receiverID
				<<": EnvAddr "	<<((envTrack*)(Temp->item))->address
				<<"]\n";

		Temp = Temp->link;
		position--;
	}
	cout<<" }\n";
}
#endif
