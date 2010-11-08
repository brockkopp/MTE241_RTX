/*
This class provides the Context functionality.
It is used to keep track of a PCB/Proc's context.
*/
#include "Context.h"
/*
	Constructor
*/
Context::Context (char* stackPtr, int stackSize) {
	//SEE rtxInitialization on UW-ACE
	jmp_buf tempBuf;
	if( setjmp(tempBuf) == 0 )
	{
		//_set_sp(stackPtr + stackSize);
		if( setjmp(local_jmp_buf ) == 0 )
			longjmp(tempBuf ,1 );
	}
}

/*
Saves state by calling the set_jmp function
*/
int Context::save() 
{
	return setjmp( (*this).local_jmp_buf );
}

/*
Saves state by calling long_jmp()
*/
void Context::restore() 
{
	longjmp( (*this).local_jmp_buf, 1);
}
