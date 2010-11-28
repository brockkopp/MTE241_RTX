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
//cout << "PROCESS A in loop!\n";	
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

//	while(true){
//		cout << "\nuserB real output\n";
//		gRTX->K_release_processor();
//	}	

	MsgEnv* myMsg;
	while(true)
	{
//cout << "PROCESS B in loop!\n";	
/* DELETE BLOCK */
//		MsgEnv* ioLetter= gRTX->K_request_msg_env(  );	
//		if ( ioLetter == NULL )
//			cout << "LETTER IS NULL\n";
//		else {
//				cout << "test1 atomic\n";
//				gRTX->K_request_delay(1,20,ioLetter);
//				cout << "test mid\n";
//				ioLetter = gRTX->K_receive_message();
//				cout << "test2\n";	
//		}
/* END BLCOK */		
		
		myMsg = gRTX->K_receive_message();

		gRTX->K_send_message(PROC_USER_C, myMsg);
		gRTX->K_release_processor();
	}
}

void userProcessC()
{
	int num = 0;
	MsgEnv* myMsg;

	while(true)
	{
		myMsg = gRTX->K_receive_message();
	
		if( myMsg!= NULL && 
				myMsg->getMsgType() == MsgEnv::COUNT_REPORT
			)
		{
			strToInt( myMsg->getMsgData(), &num );
			if(num%20 == 0 && num != 0)
			{
				
				myMsg->setMsgData("Process C\n");
				while(gRTX->K_send_console_chars(myMsg) != EXIT_SUCCESS);
				getMessage(MsgEnv::DISPLAY_ACK,gRTX);
				
				cout << "UP:start:"<<gRTX->runTime<<endl;
				gRTX->K_request_delay(100, num, myMsg);
				cout << "reqMsg\n";
				while((myMsg = getMessage(num,gRTX)) == NULL)
					gRTX->K_release_processor();
				cout << "UP:done:"<<gRTX->runTime<<endl;
				
			}
		}
	
		gRTX->K_release_msg_env(myMsg);
		gRTX->K_release_processor();
	}
}
