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
{
	//SEE rtxInitialization on UW-ACE
	jmp_buf tempBuf;

	//Init the function pointer.
	_fPtr = fp;
	cout << "ini: " << this->_localJmpBuf << endl;

	if( setjmp(tempBuf) == 0 )
	{
		//_set_sp(stackPtr + stackSize);
		if( setjmp( this->_localJmpBuf ) == 0 )
		{
			char* stkPtr = stackPtr + stackSize - 1280;
			
			__asm__("movl %0,%%esp" :"=m" (stkPtr));
			//cout << "ini: " << _localJmpBuf << endl;
			longjmp(tempBuf ,1 );
		}
		else //First time the PCB is put on CPU. Function runs here.
		{
			PCB* tmp;
			gRTX->getCurrentPcb(&tmp);
			cout << "run: " << this->_localJmpBuf << "   " << tmp->getName() << endl;
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
	debugMsg("%Context: about to SAVE context: " + tmp->getName() + "%\n");

	return setjmp( this->_localJmpBuf );
}

/*
Saves state by calling long_jmp()
*/
void Context::restore() 
{
	PCB* tmp;
	gRTX->getCurrentPcb(&tmp);
	debugMsg("%Context: about to RESTORE context: " + tmp->getName() + "%\n");
	cout << "res: " << this->_localJmpBuf << endl;

	longjmp( this->_localJmpBuf, 1);
}
