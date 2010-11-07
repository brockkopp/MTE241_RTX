/*
This class provides the Context functionality.
It is used to keep track of a PCB/Proc's context.
*/
#include <setjmp.h>
#include "../debug.h"

using namespace std;

class Context
{
    public:
	Context(char* stackPtr, int stackSize);
	int save(); 		//Save context
    	void restore(); 	//Restore context
   
   private:
    	jmp_buf local_jmp_buf; 	//Actual context, in the for of a jmp buffer
};
