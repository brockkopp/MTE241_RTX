#include <stdlib.h>
#include <iostream>

#include "jmper.h"

using namespace std;

Jmper::Jmper(int i, void (*fPtr)()) {
	
	//jmp_buf* one = (jmp_buf*)malloc(sizeof(jmp_buf));
	jmp_buf tempBuf;
	
	_fPtr = fPtr;
	
	_id = i;
	if( setjmp( tempBuf ) == 0)
	{
		if ( setjmp( _buffer ) == 0 ) 
		{
			cout << "First time (in constructor)!>" << _id << "<\n";
			longjmp( tempBuf, 1 );
		}
		else 
		{
			cout << "Second time (in constructor)>" << _id << "<\n";
			
			cout << "Starting the fPtr.........\n";
			_fPtr();
		}
	}
	else
	{
		cout << "We're back!>" << _id << "<\n";
	}
	cout << "Exiting constructor (deconstructors may be called)>" << _id << "<\n";
}

void Jmper::restore_context() {
	cout << "Context is about to be restored>" << _id << "<\n";
	longjmp( _buffer, 1 );
}
