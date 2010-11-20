#include "MsgTrace.h"

extern int gRunTime;

MsgTrace::MsgTrace()
{
		//_sendArray[16] = (TraceElement*)malloc(16);
		//_receiveArray[16] = (TraceElement*)malloc(16);
}

MsgTrace::~MsgTrace()
{
	//free(_sendArray[16]);
	//free(_receiveArray[16]);
}

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
			_sendArray[_sendArrayPosition%15]._timeStamp = gRunTime;
			_sendArrayPosition ++;
		}
		else if(callingFunction == RECEIVE)
		{
			//transfer of data from msg env to trace element circular array
			_receiveArray[_receiveArrayPosition%15]._destPid = msg->getDestPid();
			_receiveArray[_receiveArrayPosition%15]._originPid = msg->getOriginPid();
			_receiveArray[_receiveArrayPosition%15]._msgType = msg->getMsgType();
			_receiveArray[_receiveArrayPosition%15]._timeStamp = gRunTime;
			_receiveArrayPosition ++; 
		}
		return EXIT_SUCCESS;
	}
	return EXIT_ERROR;
}

MsgEnv* MsgTrace::getTraces(MsgEnv* msg)
{
	if (msg != NULL)
	{
		//initialize column headers for table display
		string tempTraceTable = "Dest PID   Origin PID   Msg Type   Time Stamp\n";
		
		string tempTableRow;
		
		for(int i=0; i<16; i++)
		{
			//constructing one row of the trace buffer table to be displayed
			tempTableRow = "   "+intToStr(_receiveArray[i]._destPid)+"            "+intToStr(_receiveArray[i]._originPid)+"          " +_receiveArray[i]._msgType+"           "+intToStr(_receiveArray[i]._timeStamp)+"\n";
			//adding above line to the trace buffer table
			tempTraceTable = tempTraceTable + tempTableRow;
		}
		//place table in msg data field
		msg->setMsgData(tempTraceTable);
		return msg;
	}	
	return NULL;
}
