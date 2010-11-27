#include "MsgTrace.h"

extern RTX* gRTX;

//CONSTRUCTOR
//Initializes circular trace arrays
MsgTrace::MsgTrace()
{
	//initialize circular arrays to store trace buffers
	_sendArray = (TraceElement*)malloc(16*sizeof(TraceElement));
	_receiveArray = (TraceElement*)malloc(16*sizeof(TraceElement));
	_sendArrayPosition = 0;
	_receiveArrayPosition = 0;
}

//DECONSTRUCTOR
//Free tracce buffer arrays
MsgTrace::~MsgTrace()
{
	//free circular arrays
	free(_sendArray);
	free(_receiveArray);
}

//Adds a trace to the buffer based on if it was sent or recieved
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

//Prepares trace buffers to be printed to the screen in table format
int MsgTrace::getTraces(MsgEnv* msg)
{
	if (msg != NULL)
	{
		//initialize column headers for table display
		string txTable = "SEND TRACE BUFFER\nINDEX\tDest\tOrigin\tType\t\tTime Sent\n";
		string rxTable = "RECEIVE TRACE BUFFER\nINDEX\tDest\tOrigin\tType\t\tTime Sent\n";
		int rxPos, txPos;
		
		for(int i=15; i >= 0; i--)
		{
			rxPos = (i + _receiveArrayPosition)%16;
			txPos = (i + _sendArrayPosition)%16;

			txTable += " " + intToStr(txPos) + \
								 "\t" + intToStr(_sendArray[txPos]._destPid) + \
								 "\t" + intToStr(_sendArray[txPos]._originPid) + \
								 "\t" + padString(getMsgTypeName(_sendArray[txPos]._msgType),12) + \
								 "\t" + intToStr(_sendArray[txPos]._timeStamp) + "\n";
				 
			rxTable += " " + intToStr(rxPos) + \
								 "\t" + intToStr(_receiveArray[rxPos]._destPid) + \
								 "\t" + intToStr(_receiveArray[rxPos]._originPid) + \
								 "\t" + padString(getMsgTypeName(_receiveArray[rxPos]._msgType),12) + \
								 "\t" + intToStr(_receiveArray[rxPos]._timeStamp) + "\n";
		}
		//place table in msg data field
		msg->setMsgData( txTable + rxTable );
		return EXIT_SUCCESS;
	}	
	return EXIT_ERROR;
}

//returns message type string for use in printing message trace buffers 
string MsgTrace::getMsgTypeName(int msgType)
{
  string ret;
	switch(msgType)
	{
		case MsgEnv::TO_CRT : ret = 						"TO_CRT"; break;
		case MsgEnv::BUFFER_OVERFLOW : ret = 		"BUF_OVFLW"; break;
		case MsgEnv::DISPLAY_ACK : ret = 				"D_ACK"; break;
		case MsgEnv::DISPLAY_FAIL : ret = 			"D_FAIL"; break;
		case MsgEnv::DELAY_REQUEST : ret = 			"D_RQST"; break;
		case MsgEnv::CONSOLE_INPUT_FIKB : ret = "FRM_FIKB"; break;
		case MsgEnv::CONSOLE_INPUT : ret = 			"FRM_KB"; break;
		case MsgEnv::COUNT_REPORT : ret = 			"CNT_RPT"; break;
		default: ret = 													"UNKNOWN"; break;
	}
	return ret;
}

//Ensures equal spacing around a string. Used for the insertion of msg types into trace buffer table
string MsgTrace::padString(string msgType, unsigned int size)
{
	const char* s = "            ";
	if(msgType.size() < size)
			return msgType.append(s,size-msgType.size());
	return msgType;
}

