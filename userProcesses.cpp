#include "userProcesses.h"

extern RTX* gRTX;

void userProcessA()
{
	//Recieve a msg
	MsgEnv* myMsg = gRTX->K_receive_message();
	gRTX->K_release_msg_env(myMsg);

	string data;
	int num = 0;
	
	while(true)
	{
		myMsg = gRTX->K_request_msg_env();
		myMsg->setMsgType(MsgEnv::COUNT_REPORT);
		data = intToStr(num);
		
		myMsg->setMsgData(data);

		gRTX->K_send_message(PROC_USER_B,myMsg);
		num++;
		gRTX->K_release_processor();
	}
}

void userProcessB()
{

	MsgEnv* myMsg;
	while(true)
	{
		myMsg = gRTX->K_receive_message();

		gRTX->K_send_message(PROC_USER_C, myMsg);
		gRTX->K_release_processor();
	}
}

void userProcessC()
{
	MsgEnv* myMsg;

	while(true)
	{
		myMsg = gRTX->K_receive_message();

		if( myMsg != NULL)
		{
			if( myMsg->getMsgType() == MsgEnv::COUNT_REPORT	)
			{
				int num = 0;
				strToInt( myMsg->getMsgData(), &num );

				if(num % 20 == 0 && num != 0)
				{
					myMsg->setMsgData("Process C\n");
					assure(gRTX->K_send_console_chars(myMsg) != EXIT_ERROR,"Send console chars failed",__FILE__,__LINE__,__func__,true);
					while( (myMsg = getAck(gRTX)) == NULL )
						debugMsg("UserC looping on Ack\n");

					gRTX->K_request_delay(100, 10, myMsg);

					while((myMsg = getMessage(10,gRTX)) == NULL)				
						gRTX->K_release_processor();				
				}
			}
			gRTX->K_release_msg_env(myMsg);
		}
		else
			debugMsg("ProcC - NULL\n");
			
		gRTX->K_release_processor();
	}
}
