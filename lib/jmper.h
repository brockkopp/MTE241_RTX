#include <setjmp.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

class Jmper {
	public:
		Jmper(int i, void (*fPtr)());
		void restore_context();
		int 	_id;	
	
	//private:
		jmp_buf _buffer;	
		
		void (*_fPtr)();
};
