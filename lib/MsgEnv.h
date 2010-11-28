#ifndef H_MSGENV
#define H_MSGENV

#include "../debug.h"
#include "../tools.h"
#include "../RTX.h"

class MsgEnv
{
	private:
		int _destPid;
		int _originPid;
		int _timeStamp;
		int _msgType;
		string _msgData;
		
	public:
		//message types		
		static const int TO_CRT =							0;
		static const int BUFFER_OVERFLOW = 		1;
		static const int DISPLAY_ACK = 				2;
		static const int DISPLAY_FAIL = 			3;
		static const int CONSOLE_INPUT_FIKB = 4;
		static const int CONSOLE_INPUT =			5;
		static const int COUNT_REPORT = 			6;
		static const int REQ_DELAY = 					7;
		static const int START_PROC =					8;

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
		static string getMsgTypeName(int msgType);
		string getMsgData();
		int setMsgData(string msgInfo);
};

#endif	
