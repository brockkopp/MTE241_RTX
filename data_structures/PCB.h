#include "rtx.h"
#include "PQ.h"
#include "Context.h"
#include "List.h"

//#include "context.h"
//#include "llist.h"
using namespace std;

class PCB {
    public:
		/*Functions*/
		PCB::PCB( int processType, int priority );
		int set_priority( int pri );
	
		/*Members*/
    int processType;
    char* stack;
    (void*) fPtr;        //Function pointer
/*
		Context context;     //Includes jmp_buf
*/
    int atomicCount;
/*    
    List mailbox;     //Message mailbox
*/    
    

	private:
		
		/*Members*/
		int id; //Process id   
		int state;
		int priority;   
};
