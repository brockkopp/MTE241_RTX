#include "MsgEnv.h"

//Message Types
//string MsgEnv::TO_CRT_F_CCI = "transmit_to_crt_request_fromCCI";
//string MsgEnv::TO_CRT_F_RTX = "transmit_to_crt_request_fromRTX";
//string MsgEnv::BUFFER_OVERFLOW = "buffer_overflow";
//string MsgEnv::DISPLAY_ACK = "display_ack";
//string MsgEnv::DISPLAY_FAIL = "display_fail";

//string MsgEnv::DELAY_REQUEST = "delay_request";

//string MsgEnv::NO_INPUT = "";
//string MsgEnv::CONSOLE_INPUT = "console_input";

//string MsgEnv::COUNT_REPORT = "count_report";

//int MsgEnv::TO_CRT_F_CCI = 	0;
//int MsgEnv::TO_CRT_F_RTX = 		1;
//int MsgEnv::BUFFER_OVERFLOW = 2;
//int MsgEnv::DISPLAY_ACK = 	3;
//int MsgEnv::DISPLAY_FAIL = 	4;
//int MsgEnv::DELAY_REQUEST = 	5;
//int MsgEnv::NO_INPUT = 		6;
//int MsgEnv::CONSOLE_INPUT = 	7;
//int MsgEnv::COUNT_REPORT = 	8;

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
		case TO_CRT_F_CCI : ret = "TO_CRT_F_CCI"; break;
		case TO_CRT_F_RTX : ret = "TO_CRT_F_RTX"; break;
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
	//debugMsg("\tgetting msg data...\n"); //ERic
	string temp = msgFields._msgData;
	//debugMsg("\tgot it\n"); //ERic
	return temp;
}

int MsgEnv::setMsgData(string msgInfo)
{
	msgFields._msgData = msgInfo;
	return EXIT_SUCCESS;
}

