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
		static string TO_CRT_F_CCI;
		static string TO_CRT_F_RTX;
		static string BUFFER_OVERFLOW;
		static string DISPLAY_ACK;
		static string DISPLAY_FAIL;

		static string WAKE_UP;
		static string DELAY_REQUEST;

		static string NO_INPUT;	
		static string CONSOLE_INPUT;
		
		static string COUNT_REPORT;

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
