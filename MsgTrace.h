#ifndef H_MSG_TRACE
#define H_MSG_TRACE

#include "lib/MsgEnv.h"

#define SEND 0
#define RECEIVE 1

class MsgTrace
{
	public:
		int addTrace(MsgEnv* msg, int callingFunction);
		MsgEnv* getTraces(MsgEnv* msg);
		
	private:
		struct TraceElement
		{
			int _destPid;
			int _originPid;
			string _msgType;
			int _timeStamp;
		}traceElement;

		TraceElement _sendArray[16];
		int _sendArrayPosition;
		TraceElement _receiveArray[16];
		int _receiveArrayPosition;
};

#endif
