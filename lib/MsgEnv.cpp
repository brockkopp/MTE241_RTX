#include "MsgEnv.h"


//Message Types
string MsgEnv::TO_CRT = "transmit_to_crt_request";

string MsgEnv::BUFFER_OVERFLOW = "buffer_overflow";
string MsgEnv::DISPLAY_ACK = "display_ack";
string MsgEnv::DISPLAY_FAIL = "display_fail";

string MsgEnv::DELAY_REQUEST = "delay_request";

string MsgEnv::NO_INPUT = "";
string MsgEnv::CONSOLE_INPUT = "console_input";

string MsgEnv::COUNT_REPORT = "count_report";

MsgEnv::MsgEnv()
{
	msgFields._destPid = -1;
	msgFields._originPid = -1;
	msgFields._timeStamp = -1;
	msgFields._msgType = " ";
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

string MsgEnv::getMsgType()
{
	return msgFields._msgType;
}

int MsgEnv::setMsgType(string newMsgType)
{
		msgFields._msgType = newMsgType;
		return EXIT_SUCCESS;
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

