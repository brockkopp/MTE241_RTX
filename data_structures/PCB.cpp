#include "rtx.h"
#include "PQ.h"
#include "Context.h"
#include "List.h"


using namespace std;

/* Constructor */

PCB::PCB ( int ProcessType, int priority) {
	_processType = processType;
	
	//Init stack <-- how to we do this?
	//init fPtr <-- how to we do this?
	Context context;
	
	_atomicCount = 0;
	
	//init List mailbox <-- class not written yet

	//init id <-- how to we do this?
	//init state <-- how to we do this?
	_priority = priority;

}

int PCB::set_priority( int pri ) {return -2}; 
