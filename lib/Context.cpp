/*
This class provides the Context functionality.
It is used to keep track of a PCB/Proc's context.
*/
#include "Context.h"
/*
	Constructor
*/

Context::Context (char* stackPtr, int stackSize, void (*fp)()) 
{
	//SEE rtxInitialization on UW-ACE
	debugMsg("%Context: Initial context restored%\n");
	jmp_buf tempBuf;

	//Init the function pointer.
	_fPtr = fp;

	if( setjmp(tempBuf) == 0 )
	{
		//_set_sp(stackPtr + stackSize);
		if( setjmp(_localJmpBuf ) == 0 ){
			debugMsg("%Context: Jumping in context constructor%\n");
			longjmp(tempBuf ,1 );
		}
		else //First time the PCB is put on CPU. Function runs here.
		{
			debugMsg("%Context: localJmpBuf returned non-zero\n");
			// Start the process for the first time.
			debugMsg("pre _fPtr call \n");
			_fPtr();
			debugMsg("post _fPtr call \n");
		}
	}
}

/*
Saves state by calling the set_jmp function
*/
int Context::save() 
{
	debugMsg("%Context: about to SAVE context%\n");
	return setjmp( _localJmpBuf );
}

/*
Saves state by calling long_jmp()
*/
void Context::restore() 
{
	debugMsg("%Context: about to RESTORE context%\n");
	longjmp( _localJmpBuf, 1);
}
