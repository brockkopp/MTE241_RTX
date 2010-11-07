/*This file implements the Priority Queue class*/
#include "PQ.h"

using namespace std;

/*
	Arguments: 
		nPriority: the number of priority levels that will be used.
*/
PQ::PQ(int nPriority) {
	Queue _master[nPriority];
}

/*
Enqueues newData into the queue with priorityLevel priority.
*/
void PQ::penqueue (PCB newData, priorityLevel)
{
	//Check to make sure that the priority level exists
	masterLength = sizeof( _master ) / sizeof( _master[0] );
	if (priorityLevel >= masterLength) {
		//Then the priorityLevel is too high, so return error
		return 1;
	}
	
	//Enqueue the data at the requested priority level.	
	return _master[ priorityLevel ].enqueue( newData );
	
}

/*
Dequeues a pointer to an pItemType. Returns null if there is nothing to dequeue.
*/
pItemType * PQ::pdequeue () 
{
	masterLength = sizeof( master ) / sizeof( master[0] );
	pItemType* returnVal;
	for( int i; i < masterLength; i++) {
		//Try dequeuing
		if (returnVal = _master[i].dequeue) {
			return returnVal;
		}	
	}
	
	return NULL; //The dequeue has failed, nothing to dequeue.	
}

