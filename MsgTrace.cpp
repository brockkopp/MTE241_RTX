#include "MsgTrace.h"

extern RTX* gRTX;

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
	if(msg != NULL)
	{
		if(callingFunction == SEND)
		{
			if(_sendArrayPosition > 15)
				_sendArrayPosition = 0;
			//transfer of data from msg env to trace element circular array
			_sendArray[_sendArrayPosition]._destPid = msg->getDestPid();  
			_sendArray[_sendArrayPosition]._originPid = msg->getOriginPid();
			_sendArray[_sendArrayPosition]._timeStamp = gRTX->runTime;
			_sendArray[_sendArrayPosition]._msgType = msg->getMsgType();
			_sendArrayPosition ++;
		}
		else if(callingFunction == RECEIVE)
		{
			if(_receiveArrayPosition > 15)
				_receiveArrayPosition = 0;
			//transfer of data from msg env to trace element circular array
			_receiveArray[_receiveArrayPosition]._destPid = msg->getDestPid();
			_receiveArray[_receiveArrayPosition]._originPid = msg->getOriginPid();
			_receiveArray[_receiveArrayPosition]._timeStamp = gRTX->runTime;
			_receiveArray[_receiveArrayPosition]._msgType = msg->getMsgType();     
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
		string txTable = "SEND TRACE BUFFER\nINDEX\tDest\tOrigin\tTime\tType\n";
		string rxTable = "RECEIVE TRACE BUFFER\nINDEX\tDest\tOrigin\tTime\tType\n";
		int rxPos, txPos;
		
		for(int i=15; i >= 0; i--)
		{
			rxPos = (i + _receiveArrayPosition)%16;
			txPos = (i + _sendArrayPosition)%16;

			txTable += " " + intToStr(txPos) + \
								 "\t" + intToStr(_sendArray[txPos]._destPid) + \
								 "\t" + intToStr(_sendArray[txPos]._originPid) + \
								 "\t" + intToStr(_sendArray[txPos]._timeStamp) + \
								 "\t" + padString(MsgEnv::getMsgTypeName(_sendArray[txPos]._msgType),12) + "\n";
								 
				 
			rxTable += " " + intToStr(rxPos) + \
								 "\t" + intToStr(_sendArray[rxPos]._destPid) + \
								 "\t" + intToStr(_sendArray[rxPos]._originPid) + \
								 "\t" + intToStr(_sendArray[rxPos]._timeStamp) + \
								 "\t" + padString(MsgEnv::getMsgTypeName(_sendArray[rxPos]._msgType),12) + "\n";
		}
		//place table in msg data field
		msg->setMsgData( txTable + rxTable );
		return EXIT_SUCCESS;
	}	
	return EXIT_ERROR;
}

string MsgTrace::padString(string msgType, unsigned int size)
{
	const char* s = "            ";
	if(msgType.size() < size)
			return msgType.append(s,size-msgType.size());
	return msgType;
}

