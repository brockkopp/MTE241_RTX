#include "libs.h"

class MsgEnv
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
			int _destPid;
			int _originPid;
			string _msgType;
			string _msgData;
		}msgFields;
};

int MsgEnv::getDestPid()
{
	//return Dest PID
	return -2;
}

int MsgEnv::setDestPid(int newDestPid)
{
	//change destPID
	return -2;
}

int MsgEnv::getOriginPid()
{
	//return origin PID
	return -2;
}

int MsgEnv::setOriginPid(int newOrignPid)
{
	//set origin_PID
	return -2;
}	

string MsgEnv::getMsgType()
{
	//return msg type
	return "-2";
}

int MsgEnv::setMsgType(string newMsgType)
{
	//set msgType
	return -2;
}

string MsgEnv::getMsgData()
{
	//return msg_data
	return "-2";
}

int MsgEnv::setMsgData(string msgInfo)
{
	//inputs data into msg_data field
	return -2;
}

