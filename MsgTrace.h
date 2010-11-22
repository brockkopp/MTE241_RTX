#ifndef H_MSG_TRACE
#define H_MSG_TRACE

#include "lib/MsgEnv.h"
#include "tools.h"
#include <stdlib.h>
#define SEND 0
#define RECEIVE 1


class MsgTrace
{
	public:
		MsgTrace();
		~MsgTrace();
		int addTrace(MsgEnv* msg, int callingFunction);
		MsgEnv* getTraces(MsgEnv* msg);
		
	private:
		struct TraceElement
		{
			int _destPid;
			int _originPid;
			string _msgType;
			int _timeStamp;
		};

		TraceElement* _sendArray;
		int _sendArrayPosition;
		TraceElement* _receiveArray;
		int _receiveArrayPosition;
};

#endif
