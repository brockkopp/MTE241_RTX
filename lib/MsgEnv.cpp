#include "MsgEnv.h"

//CONSTRUCTOR
//initializes message envelope fields 
MsgEnv::MsgEnv()
{
	msgFields._destPid = 		-1;
	msgFields._originPid = 	-1;
	msgFields._timeStamp = 	-1;
	msgFields._msgType = 		-1;
	msgFields._msgData = 		"";
}

//returns desitination PID of the message this function is called on 
int MsgEnv::getDestPid()
{
	return msgFields._destPid;
}

//sets desitination PID of the message this function is called on 
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

//returns origin PID of the message this function is called on 
int MsgEnv::getOriginPid()
{
	return msgFields._originPid;
}

//sets origin PID of the message this function is called on 
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

//returns time stamp of the message this function is called on 
int MsgEnv::getTimeStamp()
{
	return msgFields._timeStamp;
}

//sets time stamp of the message this function is called on 
int MsgEnv::setTimeStamp(int newTimeStamp)
{
	msgFields._timeStamp = newTimeStamp;
	return EXIT_SUCCESS;
}

//returns message type of the message this function is called on 
int MsgEnv::getMsgType()
{
	return msgFields._msgType;
}

//sets message type of the message this function is called on 
int MsgEnv::setMsgType(int newMsgType)
{
		msgFields._msgType = newMsgType;
		return EXIT_SUCCESS;
}

//returns message data of the message this function is called on 
string MsgEnv::getMsgData()
{
	return msgFields._msgData;
}

//sets message data of the message this function is called on 
int MsgEnv::setMsgData(string msgInfo)
{
	msgFields._msgData = msgInfo;
	return EXIT_SUCCESS;
}
