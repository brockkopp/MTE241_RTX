#include "MsgTrace.h"

extern int gRunTime;

MsgTrace::MsgTrace()
{
	//initialize circular arrays to store trace buffers
	_sendArray = (TraceElement*)malloc(16*sizeof(TraceElement));
	_receiveArray = (TraceElement*)malloc(16*sizeof(TraceElement));
	_sendArrayPosition = 0;
	_receiveArrayPosition = 0;
}

MsgTrace::~MsgTrace()
{
	//free circular arrays
	free(_sendArray);
	free(_receiveArray);
}

int MsgTrace::addTrace(MsgEnv* msg, int callingFunction)
{
	//debugMsg("\tAdding Trace...\n"); //ERic

	if(msg != NULL)
	{
		//debugMsg("\t\tmsg good\n"); //ERic
		if(callingFunction == SEND)
		{
			if(_sendArrayPosition > 15)
				_sendArrayPosition = 0;
			//debugMsg("\t\tSEND type msg\n"); //ERic
			//transfer of data from msg env to trace element circular array
			_sendArray[_sendArrayPosition]._destPid = msg->getDestPid();  
			//debugMsg("\t\tdest set\n"); //ERic
			_sendArray[_sendArrayPosition]._originPid = msg->getOriginPid();
			//debugMsg("\t\torigin set\n"); //ERic
			_sendArray[_sendArrayPosition]._timeStamp = gRunTime;
			//debugMsg("\t\ttime stamp set\n"); //ERic
			//_sendArray[_sendArrayPosition]._msgType = msg->getMsgType();     //ERic
			//debugMsg("\t\ttype set\n"); //ERic
			_sendArrayPosition ++;
		}
		else if(callingFunction == RECEIVE)
		{
			if(_receiveArrayPosition > 15)
				_receiveArrayPosition = 0;
			//debugMsg("\t\tRECEIVE type msg\n"); //ERic
			//transfer of data from msg env to trace element circular array
			_receiveArray[_receiveArrayPosition]._destPid = msg->getDestPid();
			//debugMsg("\t\tdest set\n"); //ERic
			_receiveArray[_receiveArrayPosition]._originPid = msg->getOriginPid();
			//debugMsg("\t\torigin set\n"); //ERic
			_receiveArray[_receiveArrayPosition]._timeStamp = gRunTime;
			//debugMsg("\t\ttime stamp set\n"); //ERic
			//_receiveArray[_receiveArrayPosition]._msgType = msg->getMsgType();      //ERic
			//debugMsg("\t\ttype set\n"); //ERic
			_receiveArrayPosition ++; 
		}
		else
			return EXIT_ERROR;
			
		return EXIT_SUCCESS;
	}
	return EXIT_ERROR;
}

int MsgTrace::getTraces(MsgEnv* msg)
{
	if (msg != NULL)
	{
		//initialize column headers for table display
		string tempTraceTable = "Dest PID   Origin PID   Msg Type   Time Stamp\n";
		
		string tempTableRow;
		
		for(int i=0; i<16; i++)
		{
			//constructing one row of the trace buffer table to be displayed
			tempTableRow = "   "+intToStr(_receiveArray[i]._destPid)+"            "+intToStr(_receiveArray[i]._originPid)+"       " +_receiveArray[i]._msgType+"        "+intToStr(_receiveArray[i]._timeStamp)+"\n";
			//adding above row to the trace buffer table
			tempTraceTable = tempTraceTable + tempTableRow;
		}
		//place table in msg data field
		msg->setMsgData(tempTraceTable);
		return EXIT_SUCCESS;
	}	
	return EXIT_ERROR;
}
