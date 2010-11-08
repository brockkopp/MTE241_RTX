/*This file implements the Priority Queue class*/
#include "PQ.h"

/*
	Arguments: 
		nPriority: the number of priority levels that will be used.
*/
PQ::PQ( int nPriority ) 
{
	Queue* _master = new Queue[nPriority];
	
	try
	{
		Queue q("PCB"); //create empty queue
		
		for(int i = 0; i < nPriority; i++)
			_master[i] = q;
	}
	catch(char* c)
	{
		assure(false, "Failed to create queues for the Priority Queue", __FILE__, __LINE__, __func__, true);
	}
}

PQ::~PQ()
{
	try
	{
		delete[] _master;
	}
	catch(char* c)
	{
		assure(false, "Failed to delete Priority Queue", __FILE__, __LINE__, __func__, true);
	}
}

/*
Enqueues newData into the queue with priorityLevel priority.
Return true if enqueue is successful, return false otherwise
*/
bool PQ::pq_enqueue (PCB* newData, int priorityLevel)
{
	//Check to make sure that the priority level exists
	int masterLength = sizeof(_master);
	if (priorityLevel >= masterLength) 
	{
		//Then the priorityLevel is too high, so return error
		return false;
	}
	
	//Enqueue the data at the requested priority level.	
	return (_master[priorityLevel].enqueue(newData));	
}

/*
Dequeues a pointer to an pItemType. Returns null if there is nothing to dequeue.
*/

itemType PQ::pq_dequeue() 
{
	int masterLength = sizeof(_master);
	itemType returnVal;
	for( int i; i < masterLength; i++) 
	{
		//Try dequeuing
		if (returnVal = _master[i].dequeue_PCB())
			return returnVal;
	}
	
	return NULL; //The dequeue has failed, nothing to dequeue.	
}

