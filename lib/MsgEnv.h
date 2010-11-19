#ifndef H_MSGENV
#define H_MSGENV

#include "../debug.h"
#include "../tools.h"

class MsgEnv
{
	private:
		struct MsgFields
		{
			int _destPid;
			int _originPid;
			int _timeStamp;
			string _msgType;
			string _msgData;
		}msgFields;
		
	public:
		//possible message types - Not sure implementation is correct of these constants...
		//i thought we wanted to use const int rather than strings-Eric
		static string DISPLAY_ACK;
		static string DISPLAY_FAIL;

		static string WAKE_UP;
		static string DELAY_REQUEST;
		

		static string TRANSMIT_TO_CRT_REQUEST;
		static string CONSOLE_INPUT;		


		int getDestPid();
		int setDestPid(int newDestPid);
		int getOriginPid();
		int setOriginPid(int newOriginPid);
		int getTimeStamp();
		int setTimeStamp(int newTimeStamp);
		string getMsgType();
		int setMsgType(string newMsgType);
		string getMsgData();
		int setMsgData(string msgInfo);
};

#endif	
