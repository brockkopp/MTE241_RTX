#include "PCB.h"

//

/* Constructor */

PCB::PCB(PcbInfo* info)
{
	_id = info->processId;
	_priority = info->priority;
	_processType = info->processType;
	_atomicCount = 0;
	
	//_stack    init stack (info->stackSize);
	_fPtr = info->address;
}

PCB::PCB ( int processType, int priority) 
{
	_processType = processType;
	
	//Init stack <-- how to we do this?
	//init fPtr <-- how to we do this?
	//Context context;
	
	_atomicCount = 0;
	
	//init List mailbox <-- class not written yet

	//init id <-- how to we do this?
	//init state <-- how to we do this?
	_priority = priority;
}

int PCB::set_priority( int pri ) 
{
	return -2;
}
