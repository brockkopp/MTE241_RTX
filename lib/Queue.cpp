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

bool Queue::sortedEnqueue( MsgEnv** newMsg, int timeStamp )
{
	if(newMsg == NULL)
		return false;
		
	if(_queueType == MSG_ENV)
	{		
		if(_front == NULL)			//Empty Queue
		{
			(*newMsg)->_link = NULL;
			_front = (void*)(&newMsg);	
			_rear  = (void*)(&newMsg);
		}
		else
		{
			MsgEnv* curr = (MsgEnv*)(_rear);

			if(curr->_link == NULL)								//Only 1 Node
			{
				if(curr->_timeStamp > timeStamp)
				{
					curr->_link = *newMsg;
					_front = (void*)(&newMsg);
				}
				else
				{
					(*newMsg)->_link = curr;
					_rear = (void*)(&newMsg);
				}
			}
			else if(timeStamp < ((MsgEnv*)(_front))->_timeStamp)		//Last Item
			{
				((MsgEnv*)(_front))->_link = *newMsg;
				_front = (void*)(&newMsg);
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
							_rear = (void*)(&newMsg);
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

void Queue::printQueue() 
{
	if(!isEmpty())
	{
		MsgEnv* Temp = (MsgEnv*)(_rear);
		int position = _length - 1;
	
		cout<<" { ";
	
		while(Temp != NULL)
		{
			cout<<" <-["
					<<intToStr(position) 
					<<": MsgType "			<< Temp->getMsgType()
					<<"] ";

			Temp = Temp->_link;
			position--;
		}
		cout<<" }\n";
	}
	else
		cout<<"{  }\n";
}

void Queue::printPCBQueue() 
{
	if(!isEmpty())
	{
		//cout<<__FILE__<<" : "<<__LINE__<<endl;
		PCB* Temp = (PCB*)(_rear);
	//	cout<<"Length of Queue: "<<get_length()<<"Temp is "<<(Temp == NULL ? "NULL\n" : "not NULL\n");
	//	cout<<__FILE__<<" : "<<__LINE__<<endl;
		int position = _length - 1;
	
		cout<<" { ";
	
		while(Temp != NULL)
		{
			//cout<<__FILE__<<" : "<<__LINE__<<endl;
			cout<<" <-["
					<<intToStr(position) 
					<<": ProcName "			<<Temp->getName()
					<<": ProcPri "			<<Temp->getPriority()
					<<": ProcID " 			<<Temp->getId()
					<<"] ";
			//cout<<__FILE__<<" : "<<__LINE__<<endl;
			Temp = Temp->_link;
		//	cout<<__FILE__<<" : "<<__LINE__<<endl;
			position--;
		}
		cout<<" }\n";
	}
	else
		cout<<"{  }\n";
}

#if DEBUG_MODE
void Queue::printTracker() 
{
	if(!isEmpty())
	{
		envTrack* Temp = (envTrack*)(_rear);
		int position = _length - 1;
	
		cout<<" {\n";
	
		while(Temp != NULL)
		{
			cout<<"\t<-["
					<<intToStr(position) 
					<<": A_ID "			<< Temp->allocatorID
					<<": D_ID "			<< Temp->receiverID
					<<": EnvAddr "	<< Temp->address
					<<"]\n";

			Temp = Temp->_link;
			position--;
		}
		cout<<" }\n";
	}
	cout<<"{  }\n";
}
#endif

///*This class provides the Queue functionality*/
//#include "Queue.h"
///*
//Please explain what values the qtype can be.
//*/


///*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
// *~*~*~*~*~*~*	 PRIVATE FUNCTIONS   *~*~*~*~*~*~*~*~*
// *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
////Dequeues an object from the front of the queue.
////Returns a pointer to the dequeued object.
////Returns NULL if there is no node to dequeue.
//itemType Queue::dequeue_gen()
//{
//	itemType retData = NULL;
//	
//	if (!isEmpty()) 
//	{
//		QueueNode* currentNode = _rear;

//		//Deal with special case of length = 1
//		if ( _rear == _front ) 
//		{
//			retData = _rear->item;
//			delete _rear;
//			_front = NULL;
//			_rear = NULL;
//		} 
//		else 
//		{
//			//Loop through queue until we reach the second last entry
//			for( int i = 0; i < _length - 2; i++) 
//				currentNode = currentNode->link;
//			
//			_front = currentNode;			//Set _front to second last node (last node to be deleted)
//			retData = _front->link->item;	//Save data of last node
//			
//			delete _front->link;			//Delete last node
//			_front->link = NULL;
//		}
//	  	_length--;
//	}
//	//Return old front of queue
//	return retData;	
//}

////Remove the specified value from the queue and return a pointer to it
////Return NULL if the value is not found in the queue
//itemType Queue::pluck_gen( itemType value )
//{
//	QueueNode* pluckee = _rear;
//	QueueNode* prePluckee = _rear;
//	int scanPos = _length - 1;
//	while(pluckee != NULL)
//	{
//		if( pluckee->item == value ) //remove pluckee from queue
//		{
//			if(_length == 1) //special case
//			{
//				_rear = NULL;
//				_front = NULL;
//			}
//			else
//			{
//				if(scanPos == _length - 1) //plucking tail
//					_rear = pluckee->link;
//					
//				else if(scanPos == 0) //plucking head
//					_front = prePluckee;
//					
//				prePluckee->link = pluckee->link; //cut pluckee out
//			}
//			_length--;
//			break;
//		}
//		prePluckee = pluckee;
//		pluckee = pluckee->link;
//		scanPos--;
//	}	
//	if(pluckee == NULL)
//		return NULL;
//	return pluckee->item; //if value wasn't in queue, pluckee is NULL
//}

//// Find the specified value in the queue and return a pointer to it
////Return NULL if the value is not found in the queue
//itemType Queue::select_gen( itemType value )
//{		
//	QueueNode* Temp = _rear;
//	int scanPos = _length - 1;
//	while(Temp != NULL)
//	{
//		if( Temp->item == value )
//			return Temp->item;
//		Temp = Temp->link;
//		scanPos--;
//	}
//	return NULL;
//}

///*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
// *~*~*~*~*~*~*	  PUBLIC FUNCTIONS   *~*~*~*~*~*~*~*~*
// *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/

///*~*~*~*~*~*~* CONSTRUCTOR/DESTRUCTOR *~*~*~*~*~*~*~*~*/
//Queue::Queue()
//{
//	_front = NULL;
//	_rear = NULL;	
//	_length = 0;
//	_queueType = UNK_TYPE;
//}

////Queuetypes are defined as constants in the header...
//Queue::Queue(int qtype)
//{
//	_front = NULL;
//	_rear = NULL;	
//	_length = 0;
//	_queueType = qtype;
//}

////Destructor: delete all memory used by the queue
////This destructor will automatically be called by the c++ delete function
//Queue::~Queue()
//{
// 	if(!isEmpty()) //ensure queue is not already empty
// 	{
// 		QueueNode* currNode;
// 		QueueNode* nextNode = _rear;
//		while(nextNode != NULL)
//		{
//			currNode = nextNode;
//			nextNode = currNode->link;
//			delete currNode;
//		}
// 	}
//}

///*~*~*~*~*~*~*	  ENQUEUE   *~*~*~*~*~*~*~*~*/
////Adds a node to the end of the queue containing the value parameter
////Returns true is enqueue is successful, returns false if unsuccessful (i.e. failed to allocate memory)
////NOTE: Calling funct
//bool Queue::enqueue( itemType value )
//{
//	QueueNode* Temp;
//	try{Temp = new QueueNode; }
//	catch(char* str) { return false; } //if memory allocation for the pointer fails
//	Temp->priority = -1;
//	
//	//perform required type-casting
//	if(_queueType == INT)
//		Temp->item = (int*)value;
//	else if(_queueType == STRING)
//		Temp->item = (std::string*)value;
//	else if(_queueType == PROCCONBLOCK)
//		Temp->item = (PCB*)value;	
//	else if(_queueType == MSG_ENV)
//		Temp->item = (MsgEnv*)value;
//	#if DEBUG_MODE
//		else if(_queueType == TRACKER)
//			Temp->item = (envTrack*)value;
//	#endif

//	if(_front == NULL) //adding first node
//	{ 
//		Temp->link = NULL;
//		_front = Temp;	
//		_rear	= Temp;
//	}
//	else 
//	{	
//		Temp->link = _rear; 
//		_rear = Temp;
//	}
//	_length++;
// return true;
//}


//bool Queue::sortedEnqueue( MsgEnv* newMsg, int priority )
//{
//	if(_queueType == MSG_ENV)
//	{
//		QueueNode* tempNode = new QueueNode();
//		tempNode->item = newMsg;
//		tempNode->priority = priority;
//		
//		if(_front == NULL)										//Empty Queue
//		{
//			tempNode->link = NULL;
//			_front = tempNode;	
//			_rear  = tempNode;
//		}
//		else
//		{
//			QueueNode* curr = _rear;

//			if(curr->link == NULL)								//Only 1 Node
//			{
//				if(curr->priority > tempNode->priority)
//				{
//					curr->link = tempNode;
//					_front = tempNode;
//				}
//				else
//				{
//					tempNode->link = curr;
//					_rear = tempNode;
//				}
//			}
//			else if(tempNode->priority < _front->priority)		//Last Item
//			{
//				_front->link = tempNode;
//				_front = tempNode;
//			}
//			else
//			{
//				bool done = false;
//				QueueNode* prev = NULL;			//higher
//				do
//				{
//					if(tempNode->priority > curr->priority)	//Add left
//					{
//						tempNode->link = curr;
//						if(_rear == curr)
//							_rear = tempNode;
//						else
//							prev->link = tempNode;
//						done = true;				
//					}
//					prev = curr;
//					curr = curr->link;
//				}
//				while(!done && curr != NULL);					//2+ items
//			}
//		}
//		_length++;
//		return true;
//	}

//	return false;

//}


///*~*~*~*~*~*~*	  DEQUEUE OVERLOADS   *~*~*~*~*~*~*~*~*/
//itemType Queue::dequeue_itemType()
//{
//	return dequeue_gen();
//}

//int* Queue::dequeue_int()
//{
//	return (int*)(dequeue_gen());
//}

//std::string* Queue::dequeue_string()
//{
//	return (std::string*)(dequeue_gen());
//}

//MsgEnv* Queue::dequeue_MsgEnv()
//{
//	return (MsgEnv*)(dequeue_gen());
//}

//PCB* Queue::dequeue_PCB()
//{
//	return (PCB*)(dequeue_gen());
//}

///*~*~*~*~*~*~*	  CONTAINS   *~*~*~*~*~*~*~*~*/
////Return true if the value is contained in the queue; return false otherwise
//bool Queue::contains( itemType value )
//{
//	QueueNode* Temp = _rear;
//	int position = _length - 1;
//	while(Temp != NULL)
//	{
//		if(Temp->item == value)
//			return true;
//		Temp = Temp->link;
//		position--;
//	}

//	return false;
//}

///*~*~*~*~*~*~*	  GETTERS   *~*~*~*~*~*~*~*~*/
//MsgEnv* Queue::get_front()
//{
//	if(_queueType == Queue::MSG_ENV)
//		return (MsgEnv*)_front;
//	else
//		return NULL;
//}

//int Queue::get_length()
//{
//	return _length;
//}

////Returns the queueType of the queue. If not defined(ie UNK_TYPE 0), calling function MUST PERFORM CAST TYPING when a itemType* is being returned
//int Queue::get_queueType()
//{
//	return _queueType;
//}

////Sets the queueType of the queue. May only be done if the _queueType was not previously defined
//void Queue::set_queueType( int type )
//{
//	if(_queueType == UNK_TYPE)
//		_queueType = type;
//	return;
//}

////return true if queue is empty, return false otherwise
//bool Queue::isEmpty()
//{
//	if(_front == NULL)
//		return true;
//		
//	return false;
//}

///*~*~*~*~*~*~*	  PLUCK OVERLOADS   *~*~*~*~*~*~*~*~*/
//itemType Queue::pluck(itemType value)
//{
//	return pluck_gen(value);
//}

//int* Queue::pluck(int* value)
//{
//	return (int*)pluck_gen(value);
//}

//std::string* Queue::pluck(std::string* value)
//{
//	return (std::string*)pluck_gen(value);
//}

//MsgEnv* Queue::pluck(MsgEnv* value)
//{
//	return (MsgEnv*)pluck_gen(value);
//}

//PCB* Queue::pluck(PCB* value)
//{
//	return (PCB*)pluck_gen(value);
//}

//#if DEBUG_MODE
//envTrack* Queue::pluck_Track(MsgEnv* value)
//{
//	QueueNode* pluckee = _rear;
//	QueueNode* prePluckee = _rear;
//	int scanPos = _length - 1;
//	while(pluckee != NULL)
//	{
//		if( ((envTrack*)(pluckee->item))->address == value ) //remove pluckee from queue
//		{
//			if(_length == 1) //special case
//			{
//				_rear = NULL;
//				_front = NULL;
//			}
//			else
//			{
//				if(scanPos == _length - 1) //plucking tail
//					_rear = pluckee->link;
//					
//				else if(scanPos == 0) //plucking head
//					_front = prePluckee;
//					
//				prePluckee->link = pluckee->link; //cut pluckee out
//			}
//			_length--;
//			break;
//		}
//		prePluckee = pluckee;
//		pluckee = pluckee->link;
//		scanPos--;
//	}	
//	if(pluckee == NULL)
//		return NULL;
//	return (envTrack*)pluckee->item; 
//}
//#endif

///*~*~*~*~*~*~*	  REPLACE   *~*~*~*~*~*~*~*~*/
////Find the currValue object in the queue and replace its item with newValue
////Return true if currValue does not exist in the queue; return false otherwise (if change is successful)
//bool Queue::replace( itemType currValue, itemType newValue )
//{
//	QueueNode* toChange = _rear;
//	int scanPos = _length - 1;
//	while(toChange != NULL)
//	{
//		if( toChange->item == currValue )
//			break;
//		toChange = toChange->link;
//		scanPos--;
//	}

//	if(toChange == NULL)
//		return 0;	

//	if(_queueType == INT)
//		toChange->item = (int*)newValue;
//	else if(_queueType == STRING)
//		toChange->item = (std::string*)newValue;
//	else if(_queueType == PROCCONBLOCK)
//		toChange->item = (PCB*)newValue;	
//	else if(_queueType == MSG_ENV)
//		toChange->item = (MsgEnv*)newValue;
//	else
//		toChange->item = newValue;
//	return 1;
//}

///*~*~*~*~*~*~*	  SELECT OVERLOADS   *~*~*~*~*~*~*~*~*/
//itemType Queue::select(itemType value)
//{
//	return select_gen(value);
//}

//int* Queue::select(int* value)
//{
//	return (int*)select_gen(value);
//}

//std::string* Queue::select(std::string* value)
//{
//	return (std::string*)select_gen(value);
//}

//MsgEnv* Queue::select(MsgEnv* value)
//{
//	return (MsgEnv*)select_gen(value);
//}

//PCB* Queue::select(PCB* value)
//{
//	return (PCB*)select_gen(value);
//}

///*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
// *~*~*~*~*~*~*	    FOR TESTING      *~*~*~*~*~*~*~*~*
// *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*/
// 
//string Queue::toString()
//{
//	QueueNode* Temp = _rear;
//	int position = _length - 1;
//	
//	string output = "{ ";
//	
//	while(Temp != NULL)
//	{
//		switch(_queueType)
//		{
//			case INT:
//				output += "<-[" + intToStr(position) + ": " + intToStr(*(int*)(Temp->item)) + "]";
//				break;
//			case STRING:
//				output += "<-[" + intToStr(position) + ": " +*(string*)(Temp->item) + "]";
//				break;
//			case MSG_ENV:
//				output += "<-[" + intToStr(position) + ": " + intToStr(Temp->priority) + "]";
//				break;
//			case PROCCONBLOCK:
//				output += "<-[" + intToStr(position) + ": " + ((PCB*)(Temp->item))->getName() + "]";
//				break;
//		}
//		Temp = Temp->link;
//		position--;
//	}
//	return output + " }";
//}

//void Queue::printIntQueue() //assume _typeCastType == INT
//{
//	if(_queueType == INT)
//		debugMsg("Queue: " + toString());
//}

//#if DEBUG_MODE
//void Queue::printTracker() 
//{
//	QueueNode* Temp = _rear;
//	int position = _length - 1;
//	
//	cout<<" {\n";
//	
//	while(Temp != NULL)
//	{
//		cout<<"\t<-["
//				<<intToStr(position) 
//				<<": A_ID "			<<((envTrack*)(Temp->item))->allocatorID
//				<<": D_ID "			<<((envTrack*)(Temp->item))->receiverID
//				<<": EnvAddr "	<<((envTrack*)(Temp->item))->address
//				<<"]\n";

//		Temp = Temp->link;
//		position--;
//	}
//	cout<<" }\n";
//}
//#endif
