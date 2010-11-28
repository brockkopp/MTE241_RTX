#include "MsgEnv.h"

MsgEnv::MsgEnv()
{
	_destPid = 		-1;
	_originPid = 	-1;
	_timeStamp = 	-1;
	_msgType = 		-1;
	_msgData = 		"";
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
	return _destPid;
}

int MsgEnv::setDestPid(int newDestPid)
{
	//check if a valid destPID was passed
	if(newDestPid >=0 && newDestPid <= PROCESS_COUNT)
	{
		_destPid = newDestPid;
		return EXIT_SUCCESS;
	}
	return EXIT_ERROR;
}

int MsgEnv::getOriginPid()
{
	return _originPid;
}

int MsgEnv::setOriginPid(int newOriginPid)
{
	//check if passed PID is valid
	if(newOriginPid >=0 && newOriginPid <= PROCESS_COUNT)
	{
		_originPid = newOriginPid;
		return EXIT_SUCCESS;
	}
	return EXIT_ERROR;
}	

int MsgEnv::getTimeStamp()
{
	return _timeStamp;
}

int MsgEnv::setTimeStamp(int newTimeStamp)
{
	_timeStamp = newTimeStamp;
	return EXIT_SUCCESS;
}

int MsgEnv::getMsgType()
{
	return _msgType;
}

int MsgEnv::setMsgType(int newMsgType)
{
		_msgType = newMsgType;
		return EXIT_SUCCESS;
}
string MsgEnv::getMsgTypeName()
{
	return getMsgTypeName(_msgType);
}

string MsgEnv::getMsgTypeName(int msgType)
{
  string ret;
	switch(msgType)
	{
		case -1 : ret = 								"UN_INIT"; break;
		case TO_CRT : ret = 						"TO_CRT"; break;
		case BUFFER_OVERFLOW : ret = 		"BUF_OVFLW"; break;
		case DISPLAY_ACK : ret = 				"D_ACK"; break;
		case DISPLAY_FAIL : ret = 			"D_FAIL"; break;
		case CONSOLE_INPUT_FIKB : ret = "KB_IN_IPROC"; break;
		case CONSOLE_INPUT : ret = 			"KB_IN"; break;
		case COUNT_REPORT : ret = 			"CNT_RPT"; break;
		case REQ_DELAY : ret = 					"RQ_DELAY"; break;
		case START_PROC : ret = 				"START_PROC"; break;
		default: ret = 									"USER_DEF:" + intToStr(msgType); break;
	}
	return ret;
}

string MsgEnv::getMsgData()
{
	return _msgData;
}

int MsgEnv::setMsgData(string msgInfo)
{
	_msgData = msgInfo;
	return EXIT_SUCCESS;
}
