//#include "PQ.h"
//#include "Context.h"
//#include "PQ.h"


class PCB {
	public:
		/*Functions*/
		PCB( int processType, int priority ); //Constructor
		int set_priority( int pri );
	
		/*Members*/
		int _processType;
		char* _stack;
		void* _fPtr;        
		//Context context;     //Includes jmp_buf
		int _atomicCount;
		//Queue mailbox;     //Message mailbox

	private:
		/*Members*/
		int _id; //Process id
		int _state;
		int _priority;   
};
