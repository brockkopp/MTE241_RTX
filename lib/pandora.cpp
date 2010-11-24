#include <stdlib.h>
#include <iostream>

#include "jmper.h"

using namespace std;




RTX::RTX(PcbInfo* initTable[], SignalHandler* signalHandler)
{
	for(int i=0; i < PROCESS_COUNT; i++)
	{
		_jmpList[i] = new Jmper(i,initTable[i]->address);
	}

	_jmpList[0]->restore_context();
	_jmpList[1]->restore_context();

	debugMsg("RTX Init Done",0,1);
	_jmpList[1]->restore_context();
}


class Jmper {
	public:
		Jmper(int i, void (*fPtr)());
		void restore_context();
		int 	_id;	
	
	//private:
		jmp_buf _buffer;	
		
		void (*_fPtr)();
};
Jmper::Jmper(int i, void (*fPtr)()) {
	
	//jmp_buf* one = (jmp_buf*)malloc(sizeof(jmp_buf));
	jmp_buf tempBuf;
	
	_fPtr = fPtr;
	cout << "ini:" << this << endl;
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
			cout << "pst:" << this << endl;
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
	cout << "pre:" << this << endl;
	cout << "Context is about to be restored>" << _id << "<\n";
	longjmp( _buffer, 1 );
}
