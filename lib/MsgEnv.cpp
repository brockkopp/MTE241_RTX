#include "MsgEnv.h"

MsgEnv::MsgEnv()
{
	msgFields._destPid = -1;
	msgFields._originPid = -1;
	msgFields._timeStamp = -1;
	msgFields._msgType = -1;
	msgFields._msgData = " ";
}

int MsgEnv::initMsg(int destPid, int originPid, int msgType, string msgData)
{
	if(	
			setDestPid(destPid) == EXIT_SUCCESS 			&&
			setOriginPid(originPid) == EXIT_SUCCESS   &&
			setMsgType(msgType) == EXIT_SUCCESS 			&&
			setMsgData(msgData) == EXIT_SUCCESS
		 )
					return EXIT_SUCCESS;
	return EXIT_ERROR;
}

int MsgEnv::getDestPid()
{
	return msgFields._destPid;
}

int MsgEnv::setDestPid(int newDestPid)
{
	//check if a valid destPID was passed
	if(newDestPid >=0 && newDestPid <= PROCESS_COUNT)
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
	if(newOriginPid >=0 && newOriginPid <= PROCESS_COUNT)
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

int MsgEnv::setMsgType(int newMsgType)
{
		msgFields._msgType = newMsgType;
		return EXIT_SUCCESS;
}

string MsgEnv::getMsgTypeName()
{
   string ret;
	switch(msgFields._msgType)
	{
		case TO_CRT : ret = 						"TO_CRT"; break;
		case BUFFER_OVERFLOW : ret = 		"BUF_OVFLW"; break;
		case DISPLAY_ACK : ret = 				"D_ACK"; break;
		case DISPLAY_FAIL : ret = 			"D_FAIL"; break;
		case DELAY_REQUEST : ret = 			"D_RQST"; break;
		case CONSOLE_INPUT_FIKB : ret = "CONSOLE_INPUT_FIKB"; break;
		case CONSOLE_INPUT : ret = 			"FRM_KB"; break;
		case COUNT_REPORT : ret = 			"CNT_RPT"; break;
		default: ret = 									"UNKNOWN"; break;
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
