#ifndef H_MSGENV
#define H_MSGENV

#include "../debug.h"
#include "../tools.h"
#include "../RTX.h"

class MsgEnv
{
	private:
		struct MsgFields
		{
			int _destPid;
			int _originPid;
			int _timeStamp;
			int _msgType;
			string _msgData;
		}msgFields;
		
	public:
		//message types		
		static const int TO_CRT =							0;
		static const int BUFFER_OVERFLOW = 		1;
		static const int DISPLAY_ACK = 				2;
		static const int DISPLAY_FAIL = 			3;
		static const int DELAY_REQUEST = 			4;
		static const int CONSOLE_INPUT_FIKB = 5;
		static const int CONSOLE_INPUT =			6;
		static const int COUNT_REPORT = 			7;
		static const int WAKEUP_CODE = 				8;

		MsgEnv();
		int initMsg(int destPid, int originPid, int msgType, string msgData);
		int getDestPid();
		int setDestPid(int newDestPid);
		int getOriginPid();
		int setOriginPid(int newOriginPid);
		int getTimeStamp();
		int setTimeStamp(int newTimeStamp);
		int getMsgType();
		int setMsgType(int newMsgType);
		string getMsgTypeName();
		string getMsgData();
		int setMsgData(string msgInfo);
};

#endif	
