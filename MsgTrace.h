#include "libs.h"
#include "data_structures/MsgEnv.h"

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
			int _msgType;
			int _timeStamp;
		}traceElement;

		TraceElement _sendArray[16];
		int _sendArrayPosition;
		TraceElement _receiveArray[16];
		int _receiveArrayPosition;
};
