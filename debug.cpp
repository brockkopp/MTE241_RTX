#include "debug.h"

void debugMsg(string message)
{
	//Assure that this is not in demonstration mode
#if DEBUG_MODE == 1
	cout << message;
#endif
}

void debugMsg(string message, int newLinesB, int newLinesA)
{
	stringstream msg;
	for(int i=0; i < newLinesB; i++)
		msg << "\n";
	msg << message;
	for(int i=0; i < newLinesA; i++)
		msg << "\n";
	debugMsg(msg.str());
}

int assure(bool condition, string message, string fileName, int lineNum, string func, bool isFatal)
{
	if(!condition)
	{		
		stringstream msg;
		if(isFatal)
		{
			msg << "ERROR: " << fileName << ":" << lineNum << "(" << message << ")";
			debugMsg(msg.str());
		}
		else
		{
			msg << "FATAL ERROR: " << fileName << ":" << lineNum << "(" << func << "():" << message << ")";
			debugMsg(msg.str(),1,1);
			if(func != "die")
				die(EXIT_ERROR);
			else
				debugMsg("             RTX Already Terminating",0,1);
		}
	}
	return condition;
}
