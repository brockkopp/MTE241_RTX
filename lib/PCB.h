#ifndef H_PCB
#define H_PCB
//#include "PQ.h"
//#include "Context.h"
//#include "PQ.h"
#include "PcbInfo.h"
#include "../debug.h"

class PCB 
{
	public:
		PCB(PcbInfo* tableEntry);
		PCB( int processType, int priority ); //Constructor
		int set_priority( int pri );

		//Context context;     //Includes jmp_buf
		
		//Queue mailbox;     //Message mailbox

	private:
		/*Members*/
		int _id; //Process id
		int _state;
		int _priority;   
		char* _stack;
		void* _fPtr;  
		int _atomicCount; 
		int _processType;
};
#endif
