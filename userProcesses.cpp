#include "userProcesses.h"

void userProcessA()
{
/*	MsgEnv* myMsg = K_receive_message();
	K_release_msg_env(myMsg);
	
	int num = 0;
	
	while(true)
	{
		myMsg = K_request_msg_env();
		myMsg->setMsgType(COUNT_REPORT);
		myMsg->setMsgData(num);
		K_send_message(USER_PROC_B,myMsg);
		num++;
		K_release_processor();
	}*/
}

void userProcessB()
{
	MsgEnv* myMsg;
	/*while(true)
	{
		myMsg = K_receive_message();
		K_send_message(USER_PROC_C, myMsg);
		K_release_processor();
	}*/
}

void userProcessC()
{
	/*	
	int num = 0;
	MsgEnv* myMsg;
	Queue* msgQ = new Queue("MsgEnv");

	while(true)
	{
		myMsg = K_receive_message();
		if(myMsg->getMsgData() == COUNT_REPORT)
		{
			num++;
			if(++num %20 == 0)
			{
				myMsg->setMsgType("display");
				myMsg->setMsgData("Process C");
				K_send_console_chars(myMsg);
				myMsg = K_receive_message();
				K_request_delay(100, 20, myMsg);
				//do
				//{
				//	myMsg = K_receive_message();
				//}
				//while(myMsg->getMsgData !
			}
		}
		K_release_msg_env(myMsg);

		

	}*/
}
