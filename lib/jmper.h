#include <setjmp.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

class Jmper {
	public:
		Jmper ();
		void restore_context();
	
	private:
		jmp_buf _buffer;		
};
