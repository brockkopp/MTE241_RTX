#include "CCI.h"
extern RTX* 		gRTX;
extern inputBuffer* gRxMemBuf;
extern CCI* 		gCCI;

#include <stdio.h>

CCI::CCI()
{
	wallClock = new WallClock(100000);
	userInputs = new Queue(Queue::STRING);
	
	ioLetter = gRTX->K_request_msg_env();
	assure(ioLetter != NULL, "CCI Received NULL ioLetter",__FILE__,__LINE__,__func__,true);
	
	ualarm(TICK_TIME,TICK_TIME);
}

CCI::~CCI()
{
	gRTX->K_release_msg_env(ioLetter);
	delete wallClock;
	delete userInputs;
}

int CCI::processCCI()
{
	string command;
	string input[3];
	string message;
	int params;
	
	while(true)
	{
		command = "";
		input[0] = input[1] = input[2] = "";
		message = "";	

		do
		{
			MsgEnv* testMsg = new MsgEnv();
			testMsg->setMsgType(MsgEnv::TO_CRT_F_CCI);
			testMsg->setMsgData(">RTX$ ");

//			testMsg->setMsgType(MsgEnv::TO_CRT);
//			testMsg->setMsgData(">RTX$1\n>RTX$2\n>RTX$3\n>RTX$4\n");
//			while(gRTX->displayText(testMsg) == EXIT_ERROR);
			while(gRTX->K_send_console_chars(testMsg) == EXIT_ERROR); //if exiting while loop, sure that message type is display_ack
			testMsg = gRTX->retrieveAcknowledgement(); //will receive a message
			
			assure(testMsg != NULL,"CCI:49 Failed to received message after IO dealings!",__FILE__,__LINE__,__func__,true);
						
			testMsg->setMsgData("");
			while(gRTX->K_get_console_chars(testMsg) == EXIT_ERROR)
				usleep(1000000);
				
			command = *(gCCI->userInputs->dequeue_string());
						
			//ioLetter = gRTX->K_receive_message();
			//command = ioLetter->getMsgData();
			
			//cout<<"CCI repeat command: "<<command;
						
		}while(command.length() == 0);

		params = parseString( command, input, ' ', 3);

		if(params >= 1 && params <= 3)
		{
			if(input[0] == "s")
			{
				if(params > 1)
					message = "Too many parameters for 'Send Message' command";
				else
				{
					MsgEnv* myEnv = gRTX->K_request_msg_env();
					myEnv->setDestPid(PROC_USER_A);
					myEnv->setMsgType(0);
					
					if(gRTX->K_send_message(PROC_USER_A, myEnv) != EXIT_SUCCESS)
						message = "Message failed to send";					
				}
			}
			else if(input[0] == "ps")
			{
				if(params > 1)
					message = "Too many parameters for 'Display Process Status' command";
				else
				{
					if( gRTX->K_request_process_status(ioLetter) == EXIT_SUCCESS )
						message = ioLetter->getMsgData();
					else
						message = "Request Process Status Failed\n";

				}
			}
			else if(input[0] == "c")
			{
				string time[3];				
				if(params > 2)
					message = "Too many parameters for 'Set Clock' command";
				else if(parseString(input[1],time,':',3) != 3 || wallClock->setTime(time) != EXIT_SUCCESS)
					message = "Invalid time format";
			}
			else if(input[0] == "cd")
			{
				if(params > 1)
					message = "Too many parameters for 'Display Clock' command";
				else
					wallClock->setDisplayed(true);
			}
			else if(input[0] == "ct")
			{
				if(params > 1)
					message = "Too many parameters for 'Hide Clock' command";
				else
					wallClock->setDisplayed(false);
			}
			else if(input[0] == "b")	//to complete
			{
				if(params > 1)
					message = "Too many parameters for 'Display Msg Buffers' command";
				else
				{
					cout<<"get trace buffers\n";
//					if( gRTX->K_get_trace_buffers(ioLetter) == EXIT_SUCCESS )
//						ioLetter = gRTX->K_receive_message();
//					else
//						message = "Display Trace Buffers Failed\n";

//					while(gRTX->K_send_console_chars(ioLetter) != EXIT_SUCCESS);
//					ioLetter = gRTX->K_receive_message();
				}
			}
			else if(input[0] == "t")
			{
				if(params > 1)
					message = "Too many parameters for 'Terminate' command";
				else
					kill(getpid(),SIGINT);
			}
			else if(input[0] == "n")
			{
				int pid,priority;
				PCB* pcb = NULL;
				if(strToInt(input[1],&priority) != EXIT_SUCCESS || strToInt(input[2],&pid) != EXIT_SUCCESS)
					message = "invalid parameters";
				else if(gRTX->getPcb(pid,&pcb) != EXIT_SUCCESS)
					message = "Invalid process id";
				else if(pcb->setPriority(priority) != EXIT_SUCCESS)
					message = "Invalid priority";
			}
			else if(input[0] == "scc")		//TESTING - ANGTEST
			{
				gRTX->K_send_console_chars(NULL);
			}
			else if(input[0] == "help")	//remove for demo
			{
				if(params > 1)
					message = "Too many parameters for 'Help' command";
				else
				{
					message =  "\nRTX Commands:\n";
					message += "\tSend Message            s\n";
					message += "\tProcess Status          ps\n";
					message += "\tSet Wall Clock          c hh:mm:ss\n";
					message += "\tDisplay Wall Clock      cd\n";
					message += "\tHide Wall Clock         ct\n";
					message += "\tDisplay Message Trace   b\n";
					message += "\tTerminate               t\n";
					message += "\tChange Priority         n pri pid\n\n";
				}
			}			
			else
				message = "Invalid Command Identifier: '" + input[0] + "'";
		}
		else
			message = "Invalid Command String";
		
		if(message.length() > 0)
		{
			//cout<<"\t"<<errMsg<<"\n";
//			//debugMsg("\t" + errMsg + "\n");
//			ioLetter->setMsgData("\t" + errMsg + "\n");
			ioLetter->setMsgData(message);
			gRTX->displayText(ioLetter);
			//while(gRTX->K_send_console_chars(ioLetter) != EXIT_SUCCESS);
			//ioLetter = gRTX->K_receive_message();
		}
	}	
	
	return EXIT_ERROR;
}
