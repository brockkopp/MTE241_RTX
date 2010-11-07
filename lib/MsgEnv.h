#ifndef H_MSGENV
#define H_MSGENV

#include "../debug.h"

class MsgEnv
{
	private:
		struct MsgFields
		{
			int _destPid;
			int _originPid;
			string _msgType;
			string _msgData;
		}msgFields;

	public:
		int getDestPid();
		int setDestPid(int newDestPid);
		int getOriginPid();
		int setOriginPid(int newOrignPid);
		string getMsgType();
		int setMsgType(string newMsgType);
		string getMsgData();
		int setMsgData(string msgInfo);
};

#endif	
