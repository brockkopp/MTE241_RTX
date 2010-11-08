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
			int _msgType;
			string _msgData;
		}msgFields;

	public:
		int getDestPid();
		int setDestPid(int newDestPid);
		int getOriginPid();
		int setOriginPid(int newOriginPid);
		int getMsgType();
		int setMsgType(int newMsgType);
		string getMsgData();
		int setMsgData(string msgInfo);
};

#endif	
