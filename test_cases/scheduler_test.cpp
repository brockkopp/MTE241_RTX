#include <iostream>

#include "../RTX.h"
#include "../Scheduler.h"
#include "../lib/PCB.h"
#include "../lib/PcbInfo.h"
#include "../lib/Queue.h"

using namespace std;

int main() {
	
	//Make Process
	
	PcbInfo* icurrentProc = (PcbInfo *)malloc(sizeof(PcbInfo));
	icurrentProc->name =		"currentProc";	
	icurrentProc->priority =    0;
	icurrentProc->processType = PROCESS_USER;
	icurrentProc->address = 	NULL;
	//PCB* currentProc = new PCB( icurrentProc ); 
	PCB* test ( icurrentProc ); 
	
	
//	PcbInfo* iproc1= (PcbInfo *)malloc(sizeof(PcbInfo));
//	iproc1->name =		"proc2";	
//	iproc1->priority =    1;
//	iproc1->processType = PROCESS_I;
//	iproc1->address = 	NULL;
//	PCB* proc1 = new PCB( iproc1 ); 
//	
//	PcbInfo* iproc2= (PcbInfo *)malloc(sizeof(PcbInfo));
//	iproc2->name =		"proc1";	
//	iproc2->priority =    2;
//	iproc2->processType = PROCESS_I;
//	iproc2->address = 	NULL;
//	PCB* proc2 = new PCB( iproc2 ); 
//	
//	//Make ready queue
//	Queue readyProcs ( Queue::PROCCONBLOCK );
//	
//	readyProcs.enqueue( proc1 );
//	readyProcs.enqueue( proc2 );
//	
//	//Make Scheduler
//	Scheduler scheduler (currentProc, readyProcs);
	
	
	
	
	cout << "_Done\n";
	return 0;
};
