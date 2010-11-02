#include "rtx.h"

//#include "context.h"
//#include "llist.h"
using namespace std;

class PCB {
    public:

    int processType;
    char* stack;
    (void*) fPtr;        //Function pointer
/*
		Context context;     //Includes jmp_buf
*/
    int atomicCount;
/*    
    LL_List mailbox;     //Message mailbox
*/    
    int set_priority( int pri );

	private:

		int id; //Process id   
		int state;
		int priority;   
};
