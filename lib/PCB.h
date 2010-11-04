#ifndef H_PCB
#define H_PCB
//#include "PQ.h"
//#include "Context.h"
//#include "PQ.h"
#include "PcbInfo.h"
#include "../debug.h"


//Process state constant declarations.
#define READY 0
#define BLOCKED_ENV 1
#define BLOCKED_MSG_RECIEVE 2

class PCB 
{
	public:
		PCB(PcbInfo* tableEntry);
		PCB( int processType, int priority ); //Constructor
		int set_priority( int pri );
		string name;

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
