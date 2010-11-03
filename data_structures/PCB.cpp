#include "rtx.h"
#include "PQ.h"
#include "Context.h"
#include "List.h"


using namespace std;

/* Constructor */

PCB::PCB ( int ProcessType, int priority) {
	(*this).processType = processType;
	
	//Init stack
	//init fPtr
	//init Context
	
	(*this).atomicCount = 0;
	
	//init List mailbox

	//init id
	//init state
	(*this).priority = priority;

}

int PCB::set_priority( int pri ) {return -2}; 
