/*
This class provides the Context functionality.
It is used to keep track of a PCB/Proc's context.
*/
#include "Context.h"
#include "../RTX.h"
extern RTX* gRTX;
/*
	Constructor
*/

Context::Context (char* stackPtr, int stackSize, void (*fp)()) 
{}
//{
//	//SEE rtxInitialization on UW-ACE
//	jmp_buf tempBuf;

//	//Init the function pointer.
//	_fPtr = fp;

//	if( setjmp(tempBuf) == 0 )
//	{
//		//_set_sp(stackPtr + stackSize);
//		if( setjmp(_localJmpBuf ) == 0 ){
//			char* stkPtr = stackPtr + stackSize - 4;
//			
//			__asm__ ("movl %0,%%esp" :"=m" (stkPtr));

//			longjmp(tempBuf ,1 );
//		}
//		else //First time the PCB is put on CPU. Function runs here.
//		{
//			_fPtr();
//		}
//	}
//}

void Context::init (char* stackPtr, int stackSize, void (*fp)()) 
{
	//SEE rtxInitialization on UW-ACE
	jmp_buf tempBuf;

	//Init the function pointer.
	_fPtr = fp;

	if( setjmp(tempBuf) == 0 )
	{
		//_set_sp(stackPtr + stackSize);
		if( setjmp(_localJmpBuf ) == 0 ){
			char* stkPtr = stackPtr + stackSize - 1280;
			
			__asm__("movl %0,%%esp" :"=m" (stkPtr));
			
			longjmp(tempBuf ,1 );
		}
		else //First time the PCB is put on CPU. Function runs here.
		{
			_fPtr();
		}
	}
}

/*
Saves state by calling the set_jmp function
*/
int Context::save() 
{
	PCB* tmp;
	gRTX->getCurrentPcb(&tmp);
	debugMsg("%Context: about to SAVE context: " + tmp->get_name() + "%\n");

	return setjmp( _localJmpBuf );
}

/*
Saves state by calling long_jmp()
*/
void Context::restore() 
{
	PCB* tmp;
	gRTX->getCurrentPcb(&tmp);
	debugMsg("%Context: about to RESTORE context: " + tmp->get_name() + "%\n");
	longjmp( _localJmpBuf, 1);
}
