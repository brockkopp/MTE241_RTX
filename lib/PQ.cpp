/*This file implements the Priority Queue class*/
#include "PQ.h"

/*
	Arguments: 
		nPriority: the number of priority levels that will be used.
*/
PQ::PQ( int nPriority ) 
{
	try
	{
		for(int i = 0; i < nPriority; i++)
			_master[i] = new Queue( Queue::PROCCONBLOCK ); 		//hard coded for compilability (brock)
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
bool PQ::pq_enqueue (PCB** newData, int priorityLevel)
{
	//Check to make sure that the priority level exists
	//cout<<"In PQ. NewData = ";
	//cout<<(*newData)->getName()<<endl;
	int masterLength = sizeof(_master)/sizeof(_master[0]);

	if (priorityLevel >= masterLength) 
	{
		//Then the priorityLevel is too high, so return error
		return false;
	}
	
	//Enqueue the data at the requested priority level.	
	//cout<<__FILE__<<" : "<<__LINE__<<" Before enqueue: "<<endl<<toString();
	
	bool result = _master[priorityLevel]->enqueue((void**)(newData));
	//cout<<(result == true? "Enqueue SUCCESS\n" :  "Enqueue FAIL\n");
	//cout<<__FILE__<<" : "<<__LINE__<<" Queue of priority level: "<<priorityLevel<<" :\n"<< _master[priorityLevel]->toString()<<endl;
//	cout<<__FILE__<<" : "<<__LINE__<<" Master Queue :"<<toString()<<endl;
	return (result);	
}

/*
Dequeues a pointer to an pItemType. Returns null if there is nothing to dequeue.
*/

PCB* PQ::pq_dequeue() 
{
	int masterLength = sizeof(_master)/sizeof(_master[0]);
	PCB* returnVal;
	//cout<<__FILE__<<" : "<<__LINE__<<endl;
//	cout<<"Length: "<<masterLength<<endl;
	for( int i = 0; i < masterLength; i++) 
	{
		//cout<<"Iteration "<<i<<" : "; _master[i]->printPCBQueue();
		//cout<<"Attempt dequeue...\n";
		returnVal = _master[i]->dequeue_PCB();
		if (returnVal != NULL)
		{
		//	cout<<"Found NOT NULL PCB\n";
			return returnVal; //Not sure if this should be casted here but i added it anyway to make it work -Karl
		}
		//cout<<__FILE__" : "<<__LINE__<<" Null PCB - loopy...\n";
	}
	
	return NULL; //The dequeue has failed, nothing to dequeue.	
}

//Plucks the value form the PQ. If value DNE, then it returns NULL
PCB* PQ::pq_pluck( PCB** target ) {
	int masterLength = sizeof(_master) / sizeof(_master[0]);

	//Check which queue target is in. When it is found, pluck it from the queue.
	for (int i=0; i<masterLength; i++) {
		PCB* pluckedValue = _master[i]->pluck( target );
		
		if ( pluckedValue )
			return pluckedValue;
			
		//Else, keep tryiing
	}

	//If we've gotten here, then the value did not exist in the PQ, so return NULL
	return NULL;
}

string PQ::toString()
{
	string output = "PQ:";
	int size = (int)(sizeof(_master)/sizeof(_master[0]));
	for(int i = 0; i < size; i++)
	{
		output += "\t" + _master[i]->toString() + "\n";
	}
	return output;
}

int PQ::length() {
	int length = 0;
	
	for (int i=0; i < PRIORITY_LEVELS; i++) {
	
			length += _master[i]->get_length();
	}
	
	return length;
}
