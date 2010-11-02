/*
This class provides the Context functionality.
It is used to keep track of a PCB/Proc's context.
*/
#include <setjmp.h>
#include "Context.h"


/*
	Constructor
*/
Context::Context () {
	/*
		Needs to set the jmp_buf. 
		Should be written by Brock teh Shamrock???
		
	*/
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
