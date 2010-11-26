#include "userProcesses.h"

extern RTX* gRTX;

void userProcessA()
{
//	while(true){
//		cout << "\nuserA real output\n";
//		gRTX->K_release_processor();
//	}	
	
	//Recieve a msg
	MsgEnv* myMsg = gRTX->K_receive_message();
	gRTX->K_release_msg_env(myMsg);
	
	string data;
	int num = 0;
	
	while(true)
	{
		myMsg = gRTX->K_request_msg_env();
		myMsg->setMsgType(MsgEnv::COUNT_REPORT);
		strToInt(data,&num);
		myMsg->setMsgData(data);
		gRTX->K_send_message(PROC_USER_B,myMsg);
		num++;
		gRTX->K_release_processor();
	}
}

void userProcessB()
{
//	while(true){
//		cout << "\nuserB real output\n";
//		gRTX->K_release_processor();
//	}	
	
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
//	while(true){
//		cout << "\nuserC real output\n";
//		gRTX->K_release_processor();
//	}	
	
	int num = 0;
	MsgEnv* myMsg;
	//Queue* msgQ = new Queue(Queue::MSG_ENV);

	while(true)
	{
		myMsg = gRTX->K_receive_message();
	
		if(myMsg->getMsgType() == MsgEnv::COUNT_REPORT)
		{
			strToInt( myMsg->getMsgData(), &num );
			if(num%20 == 0)
			{
//				myMsg->setMsgData("Process C");
//				while(gRTX->K_send_console_chars(myMsg) != EXIT_SUCCESS);
				cout << "Process C <-- This is a cout. Needs to be send_console_chars when CCI is implemented.\n";

				myMsg = gRTX->K_receive_message();

				cout << "Proc C going to sleep. Needs to use timing services when they are implemented\n";
				sleep(10);


//				gRTX->K_request_delay(100, 20, myMsg);
				//do
				//{
				//	myMsg = K_receive_message();
				//}
				//while(myMsg->getMsgData !
			}
		}
		gRTX->K_release_msg_env(myMsg);
		gRTX->K_release_processor();
	}
}
