#ifndef H_MSGENV
#define H_MSGENV

#include "../debug.h"
#include "../tools.h"
//#include "../RTX.h"
#define PROCESS_COUNT 7		//REMOVE!!!

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
		static string TRANSMIT_TO_CRT_REQUEST;

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
