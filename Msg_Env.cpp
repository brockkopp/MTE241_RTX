#include "rtx.h"

class Msg_Env
{
	public:
		int getDestPid();
		int setDestPid(int newDestPid);
		int getOriginPid();
		int setOriginPid(int newOrignPid);
		string getMsgType();
		int setMsgType(string newMsgType);
		string getMsgData();
		int setMsgData(string msgInfo);
	private:
		struct MsgFields
		{
			int dest_PID;
			int origin_PID;
			string msg_type;
			string msg_data;
		}msgFields;
};

int Msg_Env::getDestPid()
{
	//return Dest PID
	return -2;
}

int Msg_Env::setDestPid(int newDestPid)
{
	//change destPID
	return -2;
}

int Msg_Env::getOriginPid()
{
	//return origin PID
	return -2;
}

int Msg_Env::setOriginPid(int newOrignPid)
{
	//set origin_PID
	return -2;
}	

string Msg_Env::getMsgType()
{
	//return msg type
	return "-2";
}

int Msg_Env::setMsgType(string newMsgType)
{
	//set msgType
	return -2;
}

string Msg_Env::getMsgData()
{
	//return msg_data
	return "-2";
}

int Msg_Env::setMsgData(string msgInfo)
{
	//inputs data into msg_data field
	return -2;
}

