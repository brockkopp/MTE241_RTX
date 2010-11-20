/*
This class provides the Context functionality.
It is used to keep track of a PCB/Proc's context.
*/
#include "Context.h"
/*
	Constructor
*/
Context::Context (char* stackPtr, int stackSize) 
{
	//SEE rtxInitialization on UW-ACE
	jmp_buf tempBuf;
//	setjmp( tempBuf ); //Might this be needed? pdf doesn't say it is --karl
	if( setjmp(tempBuf) == 0 )
	{
		//_set_sp(stackPtr + stackSize);
		if( setjmp(_localJmpBuf ) == 0 )
			longjmp(tempBuf ,1 ); //Error accessing memory occuring at this line --karl
	}
}

/*
Saves state by calling the set_jmp function
*/
int Context::save() 
{
	return setjmp( _localJmpBuf );
}

/*
Saves state by calling long_jmp()
*/
void Context::restore() 
{
	longjmp( _localJmpBuf, 1);
}
