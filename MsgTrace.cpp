#include "MsgTrace.h"

int totalRunTime = 0;


int MsgTrace::addTrace(MsgEnv* msg, int callingFunction)
{
	
	if(msg != NULL)
	{
		if(callingFunction == SEND)
		{
			//transfer of data from msg env to trace element circular array
			_sendArray[_sendArrayPosition%15]._destPid = msg->getDestPid();
			_sendArray[_sendArrayPosition%15]._originPid = msg->getOriginPid();
			_sendArray[_sendArrayPosition%15]._msgType = msg->getMsgType();
			_sendArray[_sendArrayPosition%15]._timeStamp = totalRunTime;
			_sendArrayPosition ++;
		}
		else if(callingFunction == RECEIVE)
		{
			//transfer of data from msg env to trace element circular array
			_receiveArray[_receiveArrayPosition%15]._destPid = msg->getDestPid();
			_receiveArray[_receiveArrayPosition%15]._originPid = msg->getOriginPid();
			_receiveArray[_receiveArrayPosition%15]._msgType = msg->getMsgType();
			_receiveArray[_receiveArrayPosition%15]._timeStamp = totalRunTime;
			_receiveArrayPosition ++; 
		}
		return EXIT_SUCCESS;
	}
	return EXIT_ERROR;
}

MsgEnv* MsgTrace::getTraces(MsgEnv* msg)
{
	//returns msg with trace arrays arranged in tables
	return NULL;
}
