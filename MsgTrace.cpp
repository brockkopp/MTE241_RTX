#include "rtx.h"

class MsgTrace
{
	public:
		int addTrace(MsgEnv* msg, int callingFunction)
		MsgEnv* getTraces(MsgEnv* msg)
	private:
		struct TraceElement
		{
			int _destPid
			int _originPid
			int _msgType
			int _timeStamp
		};

		TraceElement _sendArray[16]
		int _sendArrayPosition
		TraceElement _recieveArray[16]
		int _receiveArrayPosition
}

int MsgTrace::addTrace(MsgEnv* msg, int callingFunction)
{
	//adds trace to correct array, _sendArray if a send function called this function, or _recieveArray if a recieve fun called
	return -2;
}

MsgEnv* MsgTrace::getTraces(MsgEnv* msg)
{
	//returns msg with trace arrays arranged in tables
	return NULL;
}
