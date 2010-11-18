#include "MsgEnv.h"

string MsgEnv::DISPLAY_ACK = "display_ack";
string MsgEnv::DISPLAY_FAIL = "display_fail";
string MsgEnv::TRANSMIT_TO_CRT_REQUEST = "transmit to CRT request";
string MsgEnv::CONSOLE_INPUT = "console_input";

int MsgEnv::getDestPid()
{
	return msgFields._destPid;
}

int MsgEnv::setDestPid(int newDestPid)
{
	//check if a valid destPID was passed
	/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * 
	~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
	~ * ~ * ~ * ~ * ~ * Using processCOUNT or no? If so, make it compile ~ * ~ * ~ * ~ * ~ * ~ * ~ * 
	~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */
	//if(newDestPid >=0 && newDestPid <= PROCESS_COUNT)//need to confirm # of processes that need to recieve
	if(newDestPid >=0 && newDestPid <= -2)
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
		/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * 
	~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
	~ * ~ * ~ * ~ * ~ * Using processCOUNT or no? If so, make it compile ~ * ~ * ~ * ~ * ~ * ~ * ~ * 
	~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */
	//if(newOriginPid >=0 && newOriginPid <=PROCESS_COUNT)//confirm # of processes
	if(newOriginPid >=0 && newOriginPid <=0)
	{
		msgFields._originPid = newOriginPid;
		return EXIT_SUCCESS;
	}
	return EXIT_ERROR;
}	

string MsgEnv::getMsgType()
{
	return msgFields._msgType;
}

int MsgEnv::setMsgType(string newMsgType)
{
	int val;
	
	if(strToInt(newMsgType,&val) == EXIT_SUCCESS && val >= 0 && val <= 3) //4 msg types
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

