#include "debug.h"

void debugMsg(string message)
{
	//Assure that this is not in demonstration mode
	#if DEBUG_MODE == 1
		cout << message;
	#endif
}

int assure(bool condition, string message, string fileName, int lineNum, bool isFatal)
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
			msg << "FATAL ERROR: " << fileName << ":" << lineNum << "(" << message << ")";
			debugMsg(msg.str());
			debugMsg("\nRTX FORCED TO TERMINATED\n\n");
			exit(1);
		}
	}
	return condition;
}
