#include <stdlib.h>
#include <iostream>

#include "jmper.h"

using namespace std;

Jmper::Jmper () {
	
	//jmp_buf* one = (jmp_buf*)malloc(sizeof(jmp_buf));

	if ( setjmp( _buffer ) == 0 ) {
		cout << "First time (in constructor)!\n";
	}
	else {
		cout << "Second time (in constructor)\n";
	}
	cout << "Exiting cnstructor (deconstructors may be called)\n";
}

void Jmper::restore_context() {
	longjmp( _buffer, 1 );
}
