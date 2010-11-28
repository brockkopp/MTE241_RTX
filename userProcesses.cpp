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

/* DELETE BLOCK */
		if( myMsg== NULL )
			cout << "THIS IS A NULL ENV being passed to B\n";
/* END DELETE */

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
		if( myMsg== NULL )
			cout << "THIS IS A NULL ENV being passed to C\n";
		
		
		if( myMsg!= NULL && 
				myMsg->getMsgType() == MsgEnv::COUNT_REPORT
			)
		{
			int num = 0;
			strToInt( myMsg->getMsgData(), &num );
			//cout<<__FILE__<<" : "<<__LINE__<<" Num = "<<num<<endl;
			if(num%20 == 0 && num != 0)
			{
				myMsg->setMsgData("Process C\n");
				
				while( gRTX->K_send_console_chars(myMsg) != EXIT_SUCCESS)
				{
					getMessage(MsgEnv::DISPLAY_ACK,gRTX);
				}
int i = rand();
cout << "Proc C going to sleep i: "<< i <<" \n";

				gRTX->K_request_delay(100, num, myMsg);
				//cout<<__FILE__<<" : "<<__LINE__<<endl;
						
				while((myMsg = getMessage(num,gRTX)) == NULL)				
					gRTX->K_release_processor();				
cout << "Proc C waking up i: "<< i <<" \n";
			}
		}
		gRTX->K_release_msg_env(myMsg);
		gRTX->K_release_processor();
	}
}
