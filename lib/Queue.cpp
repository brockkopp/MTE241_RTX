/*This class provides the Queue functionality*/
#include "Queue.h"

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
 		if(get_queueType() == MSG_ENV)
 		{
	 		MsgEnv* currNode;
	 		MsgEnv* nextNode = (MsgEnv*)(_rear);
			while(nextNode != NULL)
			{
				currNode = nextNode;
				nextNode = currNode->_link;
				delete currNode;
			}
		}
		else if(get_queueType() == PROCCONBLOCK)
 		{
	 		PCB* currNode;
	 		PCB* nextNode = (PCB*)(_rear);
			while(nextNode != NULL)
			{
				currNode = nextNode;
				nextNode = currNode->_link;
				delete currNode;
			}
		}
 	}
}

/*~*~*~*~*~*~*	  ENQUEUE   *~*~*~*~*~*~*~*~*/
//Adds a node to the end of the queue containing the value parameter
//Returns true is enqueue is successful, returns false if unsuccessful (i.e. failed to allocate memory)
bool Queue::enqueue( QueueNode* value )
{
	if( value == NULL )
		return false;
		
	//perform required type-casting
	if(_queueType == PROCCONBLOCK)
	{
		if(_front == NULL) //adding first node
			((PCB*)(*value))->_link = NULL;
		else 
			((PCB*)(*value))->_link = (PCB*)(_rear); 
	}
	else if(_queueType == MSG_ENV)
	{
			if(_front == NULL) //adding first node
				((MsgEnv*)(*value))->_link = NULL;
			else 
				((MsgEnv*)(*value))->_link = (MsgEnv*)(_rear); 
	}
	#if DEBUG_MODE
		else if(_queueType == TRACKER)
		{
			if(_front == NULL) //adding first node
				((envTrack*)(*value))->_link = NULL;
			else 
				((envTrack*)(*value))->_link = (envTrack*)(_rear);
		}
	#endif

	if( _front == NULL )
	{
		_front = *value;
		_rear = *value;
	}
	else
		_rear = *value;
			
		_length++;
	 return true;
}

bool Queue::sortedEnqueue( MsgEnv** newMsg )
{

	int timeStamp = (*newMsg)->_timeStamp;

	if(newMsg == NULL)
		return false;
		
	if(_queueType == MSG_ENV)
	{		
		if(_front == NULL)			//Empty Queue
		{
			(*newMsg)->_link = NULL;
			_front = (void*)(*newMsg);	
			_rear  = (void*)(*newMsg);
		}
		else
		{
			MsgEnv* curr = (MsgEnv*)(_rear);

			if(curr->_link == NULL)								//Only 1 Node
			{
				if(curr->_timeStamp > timeStamp)
				{
					curr->_link = *newMsg;
					_front = (void*)(*newMsg);
				}
				else
				{
					(*newMsg)->_link = curr;
					_rear = (void*)(*newMsg);
				}
			}
			else if(timeStamp < ((MsgEnv*)(_front))->_timeStamp)		//Last Item
			{
				((MsgEnv*)(_front))->_link = *newMsg;
				_front = (void*)(*newMsg);
			}
			else
			{
				bool done = false;
				MsgEnv* prev = NULL;			//higher
				do
				{
					if(timeStamp > curr->_timeStamp)	//Add left
					{
						(*newMsg)->_link = curr;
						if(_rear == (void*)(curr))
							_rear = (void*)(*newMsg);
						else
							prev->_link = *newMsg;
						done = true;				
					}
					prev = curr;
					curr = curr->_link;
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
//Dequeues an object from the front of the queue.
//Returns a pointer to the dequeued object.
//Returns NULL if there is no node to dequeue.
MsgEnv* Queue::dequeue_MsgEnv()
{
	MsgEnv* retData = NULL;
	
	if (!isEmpty()) 
	{
		MsgEnv** currentNode = (MsgEnv**)(&(_rear));

		//Deal with special case of length = 1
	//	if ( get_length() == 1 ) 
		if( _rear == _front )
		{
			retData = *currentNode;
			_front = NULL;
			_rear = NULL;
		} 
		else 
		{
			//Loop through queue until we reach the second last entry
			for( int i = 0; i < _length - 2; i++) 
			{
				currentNode = &((*currentNode)->_link);
			}
			
			_front = (void*)(*currentNode);				//Set _front to second last node (last node to be deleted)
			retData = ((MsgEnv*)(_front))->_link;	//Save data of last node
			
			((MsgEnv*)(_front))->_link = NULL; //clear last node
		}
	  	_length--;
	}
	//Return old front of queue
	return retData;	
}

PCB* Queue::dequeue_PCB()
{
	PCB* retData = NULL;
	
	if (!isEmpty()) 
	{
		PCB** currentNode = (PCB**)(&(_rear));

		//Deal with special case of length = 1
		//	if ( get_length() == 1 ) 
		if( _rear == _front )
		{
			retData = *currentNode;
			_front = NULL;
			_rear = NULL;
		} 
		else 
		{
			//Loop through queue until we reach the second last entry
			for( int i = 0; i < _length - 2; i++) 
			{
				currentNode = &((*currentNode)->_link);
			}
			
			_front = (void*)(*currentNode);				//Set _front to second last node (last node to be deleted)
			retData = ((PCB*)(_front))->_link;	//Save data of last node
			
			((PCB*)(_front))->_link = NULL; //clear last node
		}
	  	_length--;
	}
	//Return old front of queue
	return retData;	
}

/*~*~*~*~*~*~*	  CONTAINS   *~*~*~*~*~*~*~*~*/
//Return true if the value is contained in the queue; return false otherwise
bool Queue::contains( QueueNode* value )
{
	if(!isEmpty() && value != NULL)
	{
		if(get_queueType() == MSG_ENV)
		{
			MsgEnv* Temp = (MsgEnv*)(_rear);
			
			int position = _length - 1;
			while(Temp != NULL)
			{
				if(Temp == *value)
					return true;
				Temp = Temp->_link;
				position--;
			}
		}
		else if(get_queueType() == PROCCONBLOCK)
		{
			PCB* Temp = (PCB*)(_rear);
			
			int position = _length - 1;
			while(Temp != NULL)
			{
				if(Temp == *value)
					return true;
				Temp = Temp->_link;
				position--;
			}
		}		
	}
	return false;
}

/*~*~*~*~*~*~*	  GETTERS   *~*~*~*~*~*~*~*~*/
MsgEnv* Queue::get_front()
{
	if(_queueType == Queue::MSG_ENV && !isEmpty())
		return (MsgEnv*)(_front);
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
//Remove the specified value from the queue and return a pointer to it
//Return NULL if the value is not found in the queue
MsgEnv* Queue::pluck(MsgEnv** value)
{
	if(!isEmpty() && value != NULL)
	{
		MsgEnv* pluckee = (MsgEnv*)(_rear);
		MsgEnv* prePluckee = (MsgEnv*)(_rear);
		int scanPos = _length - 1;
		while(pluckee != NULL)
		{
			if( pluckee == *value ) //remove pluckee from queue
			{
				if(_length == 1) //special case
				{
					_rear = NULL;
					_front = NULL;
				}
				else
				{
					if(scanPos == _length - 1) //plucking tail
						_rear = (void*)(pluckee->_link);
				
					else if(scanPos == 0) //plucking head
						_front = (void*)(prePluckee);
				
					prePluckee->_link = pluckee->_link; //cut pluckee out
				}
				_length--;
				break;
			}
			prePluckee = pluckee;
			pluckee = pluckee->_link;
			scanPos--;
		}	
		if(pluckee == NULL)
			return NULL;
			
		return pluckee; //if value wasn't in queue, pluckee is NULL
	}
	return NULL;
}

PCB* Queue::pluck(PCB** value)
{
	if(!isEmpty() && value != NULL)
	{
		PCB* pluckee = (PCB*)(_rear);
		PCB* prePluckee = (PCB*)(_rear);
		int scanPos = _length - 1;
		while(pluckee != NULL)
		{
			if( pluckee == *value ) //remove pluckee from queue
			{
				if(_length == 1) //special case
				{
					_rear = NULL;
					_front = NULL;
				}
				else
				{
					if(scanPos == _length - 1) //plucking tail
						_rear = (void*)(pluckee->_link);
				
					else if(scanPos == 0) //plucking head
						_front = (void*)(prePluckee);
				
					prePluckee->_link = pluckee->_link; //cut pluckee out
				}
				_length--;
				break;
			}
			prePluckee = pluckee;
			pluckee = pluckee->_link;
			scanPos--;
		}	
		if(pluckee == NULL)
			return NULL;
			
		return pluckee; //if value wasn't in queue, pluckee is NULL
	}
	return NULL;
}

#if DEBUG_MODE
envTrack* Queue::pluck_Track(MsgEnv** value)
{
	if(!isEmpty() && value != NULL && get_queueType() == TRACKER)
	{
		envTrack* pluckee = (envTrack*)(_rear);
		envTrack* prePluckee = (envTrack*)(_rear);
		int scanPos = _length - 1;
		while(pluckee != NULL)
		{
			if( pluckee->address == *value ) //remove pluckee from queue
			{
				if(_length == 1) //special case
				{
					_rear = NULL;
					_front = NULL;
				}
				else
				{
					if(scanPos == _length - 1) //plucking tail
						_rear = (void*)(pluckee->_link);
					
					else if(scanPos == 0) //plucking head
						_front = (void*)(prePluckee);
					
					prePluckee->_link = pluckee->_link; //cut pluckee out
				}
				_length--;
				break;
			}
			prePluckee = pluckee;
			pluckee = pluckee->_link;
			scanPos--;
		}	
		if(pluckee == NULL)
			return NULL;
		return pluckee; 
	}
	return NULL;
}
#endif

/*~*~*~*~*~*~*	  REPLACE   *~*~*~*~*~*~*~*~*/
//Find the currValue object in the queue and replace its item with newValue
//Return true if currValue does not exist in the queue; return false otherwise (if change is successful)
//newValue CANNOT be null because we cannot lose the link to the rest of the nodes in the queue
bool Queue::replace( QueueNode* currValue, QueueNode* newValue )
{
	if(!isEmpty() && currValue != NULL && newValue != NULL)
	{
		if(_queueType == PROCCONBLOCK)
		{
			PCB* toChange = (PCB*)(_rear);
			PCB* prev = (PCB*)(_rear);
			int scanPos = _length - 1;
			while(toChange != NULL)
			{
				if( toChange == *currValue )
					break;
				prev = toChange;
				toChange = toChange->_link;
				scanPos--;
			}
			
			if(toChange == NULL)
				return false;	
			
			//insert new pointer in the queue
			if(get_length() == 1) //special case
			{
				_front = *newValue;
				_rear = *newValue;
				((PCB*)(*newValue))->_link = NULL;
			}
			else if(scanPos == _length - 1) //replacing tail node
			{
				_rear = *newValue;
				((PCB*)(*newValue))->_link = prev->_link;
			}
			else if(scanPos == 0) //replacing front node
			{
				prev->_link = (PCB*)(*newValue);
				((PCB*)(*newValue))->_link = NULL;
				_front = *newValue;
			}
			else
			{
				prev->_link = (PCB*)(*newValue);
				((PCB*)(*newValue))->_link = toChange->_link;
			}
		}
		else if(_queueType == MSG_ENV)
		{
			MsgEnv* toChange = (MsgEnv*)(_rear);
			MsgEnv* prev = (MsgEnv*)(_rear);
			int scanPos = _length - 1;
			while(toChange != NULL)
			{
				if( toChange == *currValue )
					break;
				prev = toChange;
				toChange = toChange->_link;
				scanPos--;
			}
			
			if(toChange == NULL)
				return false;	
			
			//insert new pointer in the queue
			if(get_length() == 1) //special case
			{
				_front = *newValue;
				_rear = *newValue;
				((MsgEnv*)(*newValue))->_link = NULL;
			}
			else if(scanPos == _length - 1) //replacing tail node
			{
				_rear = *newValue;
				((MsgEnv*)(*newValue))->_link = prev->_link;
			}
			else if(scanPos == 0) //replacing front node
			{
				prev->_link = (MsgEnv*)(*newValue);
				((MsgEnv*)(*newValue))->_link = NULL;
				_front = *newValue;
			}
			else
			{
				prev->_link = (MsgEnv*)(*newValue);
				((MsgEnv*)(*newValue))->_link = toChange->_link;
			}
		}
		else
			return false;
		
		return true;
	}
	return false;
}

/*~*~*~*~*~*~*	  SELECT OVERLOADS   *~*~*~*~*~*~*~*~*/
// Find the specified value in the queue and return a pointer to it
//Return NULL if the value is not found in the queue
MsgEnv* Queue::select(MsgEnv** value)
{
	if(!isEmpty() && value != NULL)
	{
		MsgEnv* Temp = (MsgEnv*)(_rear);
		int scanPos = _length - 1;
		while(Temp != NULL)
		{
			if( Temp == *value )
				return Temp;
			Temp = Temp->_link;
			scanPos--;
		}
	}
	return NULL;
}

PCB* Queue::select(PCB** value)
{
	if(!isEmpty() && value != NULL)
	{
		PCB* Temp = (PCB*)(_rear);
		int scanPos = _length - 1;
		while(Temp != NULL)
		{
			if( Temp == *value )
				return Temp;
			Temp = Temp->_link;
			scanPos--;
		}
	}
	return NULL;
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
 *~*~*~*~*~*~*	    FOR TESTING      *~*~*~*~*~*~*~*~*
 *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
 
string Queue::toString()
{
	if(!isEmpty())
	{
		if(get_queueType() == PROCCONBLOCK)
		{
			string output = "{ ";
			PCB* Temp = (PCB*)(_rear);
			int position = _length - 1;
		
			while(Temp != NULL)
			{
				output += (" <-["
						 + intToStr(position) 
						 + ": ProcName "		+ Temp->getName()
						 + ": ProcPri "			+ intToStr(Temp->getPriority())
						 + ": ProcID " 			+ intToStr(Temp->getId())
						 + "] \n");
				Temp = Temp->_link;
				position--;
			}
			output += " }\n";
			return (output);
		}
		else if(get_queueType() == MSG_ENV)
		{
			string output = "{ ";
			MsgEnv* Temp = (MsgEnv*)(_rear);
			int position = _length - 1;
		
			while(Temp != NULL)
			{
				output += (" <-["
						 + intToStr(position) 
						 + ": TimeStamp "		+ intToStr(Temp->_timeStamp)
						 + "] ");
				Temp = Temp->_link;
				position--;
			}
			output += " }\n";
			return (output);
		}
	}
	return ("{  }\n");
}
