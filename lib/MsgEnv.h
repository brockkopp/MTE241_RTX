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
		//possible message types - Not sure implementation is correct of these constants...
		static string DISPLAY_ACK;
		static string DISPLAY_FAIL;

		int getDestPid();
		int setDestPid(int newDestPid);
		int getOriginPid();
		int setOriginPid(int newOriginPid);
		string getMsgType();
		int setMsgType(string newMsgType);
		string getMsgData();
		int setMsgData(string msgInfo);
};

#endif	
