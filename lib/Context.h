/*
This class provides the Context functionality.
It is used to keep track of a PCB/Proc's context.
*/
#ifndef H_CONTEXT
#define H_CONTEXT

#include "../debug.h"
#include <setjmp.h>

class Context
{
    public:
		//Context(char* stackPtr, int stackSize, void* fxn_ptr);
		Context (char* stackPtr, int stackSize, void (*fp)());
		int save(); 		//Save context
		void restore(); 	//Restore context
		void init (char* stackPtr, int stackSize, void (*fp)());
   
   private:
    	jmp_buf _localJmpBuf; 	//Actual context, in the for of a jmp buffer
    	void (*_fPtr) ();
};
#endif
