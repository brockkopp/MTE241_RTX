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
			ioLetter->setMsgData(">RTX$ ");
			while(gRTX->K_send_console_chars(ioLetter) == EXIT_ERROR); //; //if exiting while loop, sure that message type is display_ack
//			{
//				//cout<<"loopy\n";
//			}
			ioLetter = gRTX->retrieveOutAcknowledgement(); //will receive a message
			
			assure(ioLetter != NULL,"CCI:46 Failed to receive message after IO dealings!",__FILE__,__LINE__,__func__,true);
						
			ioLetter->setMsgData("");
			while(gRTX->K_get_console_chars(ioLetter) == EXIT_ERROR)
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
					message = "Too many parameters for 'Send Message' command\n";
				else
				{
					MsgEnv* myEnv = gRTX->K_request_msg_env();
					myEnv->setDestPid(PROC_USER_A);
					myEnv->setMsgType(0);
					
					if(gRTX->K_send_message(PROC_USER_A, myEnv) != EXIT_SUCCESS)
						message = "Message failed to send\n";	
					else
						message = "Sent message!\n";				
				}
			}
			else if(input[0] == "ps")
			{
				if(params > 1)
					message = "Too many parameters for 'Display Process Status' command\n";
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
					message = "Too many parameters for 'Set Clock' command\n";
				else if(parseString(input[1],time,':',3) != 3 || wallClock->setTime(time) != EXIT_SUCCESS)
					message = "Invalid time format\n";
			}
			else if(input[0] == "cd")
			{
				if(params > 1)
					message = "Too many parameters for 'Display Clock' command\n";
				else
					wallClock->setDisplayed(true);
			}
			else if(input[0] == "ct")
			{
				if(params > 1)
					message = "Too many parameters for 'Hide Clock' command\n";
				else
					wallClock->setDisplayed(false);
			}
			else if(input[0] == "b")	//to complete
			{
				if(params > 1)
					message = "Too many parameters for 'Display Msg Buffers' command\n";
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
					message = "Too many parameters for 'Terminate' command\n";
				else
					kill(getpid(),SIGINT);
			}
			else if(input[0] == "n")
			{
				int pid,priority;
				PCB* pcb = NULL;
				if(strToInt(input[1],&priority) != EXIT_SUCCESS || strToInt(input[2],&pid) != EXIT_SUCCESS)
					message = "invalid parameters\n";
				else if(gRTX->getPcb(pid,&pcb) != EXIT_SUCCESS)
					message = "Invalid process id\n";
				else if(pcb->setPriority(priority) != EXIT_SUCCESS)
					message = "Invalid priority\n";
			}
			else if(input[0] == "scc")		//TESTING - ANGTEST
			{
				gRTX->K_send_console_chars(NULL);
			}
			else if(input[0] == "help")	//remove for demo
			{
				if(params > 1)
					message = "Too many parameters for 'Help' command\n";
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
				message = "Invalid Command Identifier: '" + input[0] + "'\n";
		}
		else
			message = "Invalid Command String\n";
		
		if(message.length() > 0)
		{
			ioLetter->setMsgData(message);
			//cout<<"CCI: send console chars\n";
			
			int result = EXIT_SUCCESS;
			do
			{
				//cout<<"Before: contents of msg: "<<ioLetter->getMsgData()<<endl<<flush;
				result = gRTX->K_send_console_chars(ioLetter);
				//cout<<"After one attempt at send, result is " << ((result==EXIT_SUCCESS)?"SUCCESS!\t":"FAILURE :(\t") <<"Contents of msg: "<<ioLetter->getMsgData()<<endl;
			}while(result == EXIT_ERROR);//; //if exiting while loop, sure that message type is display_ack
			
			//cout<<"Sent console chars. Waiting for acknowledgement\n";
			ioLetter = gRTX->retrieveOutAcknowledgement(); //will receive a message
			//cout<<"Received acknowledgement\n";
			
			assure(ioLetter != NULL,"CCI:182 Failed to receive message after IO dealings!",__FILE__,__LINE__,__func__,true);
			//cout<<"Assured ioLetter isn't null\n";
			
			//gRTX->displayText(ioLetter);
		}
		usleep(100000);
	}	
	
	return EXIT_ERROR;
}
