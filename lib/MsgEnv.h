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
			int _msgType;
			string _msgData;
		}msgFields;
		
	public:
		//message types
//		static string TO_CRT_F_CCI;
//		static string TO_CRT_F_RTX;
//		static string BUFFER_OVERFLOW;
//		static string DISPLAY_ACK;
//		static string DISPLAY_FAIL;

//		static string DELAY_REQUEST;

//		static string NO_INPUT;	
//		static string CONSOLE_INPUT;
//		
//		static string COUNT_REPORT;
		
		static const int TO_CRT_F_CCI =		0;
		static const int TO_CRT_F_RTX = 	1;
		static const int BUFFER_OVERFLOW = 	2;
		static const int DISPLAY_ACK = 		3;
		static const int DISPLAY_FAIL = 	4;
		static const int DELAY_REQUEST = 	5;
		static const int NO_INPUT = 		6;	
		static const int CONSOLE_INPUT = 	7;
		static const int COUNT_REPORT = 	8;

		int getDestPid();
		int setDestPid(int newDestPid);
		int getOriginPid();
		int setOriginPid(int newOriginPid);
		int getTimeStamp();
		int setTimeStamp(int newTimeStamp);
		int getMsgType();
		string getMsgTypeName();
		//void setMsgType(string newMsgType);
		void setMsgType(int newMsgType);
		string getMsgData();
		int setMsgData(string msgInfo);
};

#endif	
