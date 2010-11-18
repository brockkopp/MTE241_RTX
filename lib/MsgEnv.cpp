#include "MsgEnv.h"

string MsgEnv::DISPLAY_ACK = "display_ack";
string MsgEnv::DISPLAY_FAIL = "display_fail";
string MsgEnv::WAKE_UP = "wake_up";
string MsgEnv::DELAY_REQUEST = "delay_request";


int MsgEnv::getDestPid()
{
	return msgFields._destPid;
}

int MsgEnv::setDestPid(int newDestPid)
{
	//check if a valid destPID was passed
	if(newDestPid >=0 && newDestPid <=PROCESS_COUNT)
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
	if(newOriginPid >=0 && newOriginPid <=PROCESS_COUNT)//confirm # of processes
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

string MsgEnv::getMsgType()
{
	return msgFields._msgType;
}

int MsgEnv::setMsgType(string newMsgType)
{
	
	if(newMsgType == DISPLAY_ACK || newMsgType == DISPLAY_FAIL || newMsgType == WAKE_UP || newMsgType == DELAY_REQUEST) 
	{
		msgFields._msgType = newMsgType;
		return EXIT_SUCCESS;
	}
	
	return EXIT_ERROR;
	
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

