#include "MsgEnv.h"

//Message Types
//int MsgEnv::TO_CRT = 			0;
//int MsgEnv::BUFFER_OVERFLOW = 1;
//int MsgEnv::DISPLAY_ACK = 	2;
//int MsgEnv::DISPLAY_FAIL = 	3;
//int MsgEnv::DELAY_REQUEST = 	4;
//int MsgEnv::NO_INPUT = 		5;
//int MsgEnv::CONSOLE_INPUT = 	6;
//int MsgEnv::COUNT_REPORT = 	7;

MsgEnv::MsgEnv()
{
	msgFields._destPid = -1;
	msgFields._originPid = -1;
	msgFields._timeStamp = -1;
	msgFields._msgType = -1;
	msgFields._msgData = " ";
}

MsgEnv::~MsgEnv()
{
	//do i need anything here?
}

int MsgEnv::getDestPid()
{
	return msgFields._destPid;
}

int MsgEnv::setDestPid(int newDestPid)
{
	//check if a valid destPID was passed
	if(newDestPid >=0 && newDestPid <=7)           //PROCESS_COUNT)
	{
		msgFields._destPid = newDestPid;
		return EXIT_SUCCESS;
	}
	return EXIT_ERROR;
}

int MsgEnv::getOriginPid()
{
	return msgFields._originPid;
}

int MsgEnv::setOriginPid(int newOriginPid)
{
	//check if passed PID is valid
	if(newOriginPid >=0 && newOriginPid <= 7)           //PROCESS_COUNT)
	{
		msgFields._originPid = newOriginPid;
		return EXIT_SUCCESS;
	}
	return EXIT_ERROR;
}	

int MsgEnv::getTimeStamp()
{
	return msgFields._timeStamp;
}

int MsgEnv::setTimeStamp(int newTimeStamp)
{
	msgFields._timeStamp = newTimeStamp;
	return EXIT_SUCCESS;
}

int MsgEnv::getMsgType()
{
	return msgFields._msgType;
}


void MsgEnv::setMsgType(int newMsgType)
{
		msgFields._msgType = newMsgType;
}

string MsgEnv::getMsgTypeName()
{
   string ret;
	switch(msgFields._msgType)
	{
		case TO_CRT : ret = "TO_CRT"; break;
		case BUFFER_OVERFLOW : ret = "BUF_OVFLW"; break;
		case DISPLAY_ACK : ret = "D_ACK"; break;
		case DISPLAY_FAIL : ret = "D_FAIL"; break;
		case DELAY_REQUEST : ret = "D_RQST"; break;
		case NO_INPUT : ret = "NO_INPT"; break;
		case CONSOLE_INPUT : ret = "FRM_KB"; break;
		case COUNT_REPORT : ret = "CNT_RPT"; break;
		default: ret = "UNKNOWN"; break;
	}
	return ret;
}

string MsgEnv::getMsgData()
{
	return msgFields._msgData;
}

int MsgEnv::setMsgData(string msgInfo)
{
	msgFields._msgData = msgInfo;
	return EXIT_SUCCESS;
}

