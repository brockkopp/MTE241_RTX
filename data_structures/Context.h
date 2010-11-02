/*
This class provides the Context functionality.
It is used to keep track of a PCB/Proc's context.
*/
#include <setjmp.h>

using namespace std;

class Context
{
    public:

			save();

    restore();
   
   		private():
    	jmp_buf jump_buf();
};
