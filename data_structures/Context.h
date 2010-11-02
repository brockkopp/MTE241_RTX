/*
This class provides the Context functionality.
It is used to keep track of a PCB/Proc's context.
*/
#include <setjmp.h>

using namespace std;

class Context
{
    public:

			int save(); //Save context

    	void restore(); //Restor context
   
   	private:
   		Context(); //Constructor
    	jmp_buf local_jmp_buf; //Actual context, in the for of a jmp buffer
};
