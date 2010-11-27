#include "CCI.h"

extern RTX* gRTX;

void processCCI()
{
	if(assure(gRTX != NULL, "CCI failed to start. gRTX NULL",__FILE__,__LINE__,__func__,false))
	{
		string command;
		string input[3];
		string message;
		int params;
		MsgEnv* ioLetter = gRTX->K_request_msg_env();
		ioLetter->setOriginPid(gRTX->getCurrentPid());

		assure(ioLetter != NULL, "CCI ioLetter is NULL",__FILE__,__LINE__,__func__,true);

		ualarm(TICK_TIME, TICK_TIME);
		
		while(true)
		{
			command = "";
			input[0] = input[1] = input[2] = "";
			message = "";	
			
			ioLetter->setMsgData(">RTX$ ");
			while(gRTX->K_send_console_chars(ioLetter) == EXIT_ERROR); //if exiting while loop, sure that message type is display_ack

			ioLetter = getMessage(MsgEnv::DISPLAY_ACK,gRTX);

			assure(ioLetter != NULL,"CCI:45 Failed to receive message after IO dealings!",__FILE__,__LINE__,__func__,true);
	
			ioLetter->setMsgData("");
			while(gRTX->K_get_console_chars(ioLetter) == EXIT_ERROR)
				usleep(100000); //no user input provided yet. Wait!	
			ioLetter->setOriginPid(gRTX->getCurrentPid());
			ioLetter = gRTX->K_receive_message(); 
			assure(ioLetter != NULL,"CCI:53 Failed to receive message after IO dealings!",__FILE__,__LINE__,__func__,true);					
			command = ioLetter->getMsgData();	
				
			//check for an empty command!
			if(command.length() > 0)
			{
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
						else if(parseString(input[1],time,':',3) != 3 || gRTX->wallClock->setTime(time) != EXIT_SUCCESS)
							message = "Invalid time format\n";
					}
					else if(input[0] == "cd")
					{
						if(params > 1)
							message = "Too many parameters for 'Display Clock' command\n";
						else
							gRTX->wallClock->setDisplayed(true);
					}
					else if(input[0] == "ct")
					{
						if(params > 1)
							message = "Too many parameters for 'Hide Clock' command\n";
						else
							gRTX->wallClock->setDisplayed(false);
					}
					else if(input[0] == "b")	//to complete
					{
						if(params > 1)
							message = "Too many parameters for 'Display Msg Buffers' command\n";
						else
						{
							if( gRTX->K_get_trace_buffers(ioLetter) != EXIT_SUCCESS )
								message = "Display Trace Buffers Failed\n";
							else
							{
								while(gRTX->K_send_console_chars(ioLetter) != EXIT_SUCCESS);
								ioLetter = gRTX->K_receive_message();
								message = "";
							}
						}
					}
					else if(input[0] == "t")
					{
						if(params > 1)
							message = "Too many parameters for 'Terminate' command\n";
						else
						{
							gRTX->K_release_msg_env(ioLetter);
							kill(getpid(),SIGINT);
						}
					}
					else if(input[0] == "n")
					{
						int pid,priority;
						PCB* pcb = NULL;			
						if(params < 3)
							message = "Not enough parameters for 'Change Priority' Command\n";		
						else if(strToInt(input[1],&priority) != EXIT_SUCCESS || strToInt(input[2],&pid) != EXIT_SUCCESS)
							message = "Invalid parameters\n";
						else if(gRTX->getPcb(pid,&pcb) != EXIT_SUCCESS)
							message = "Invalid process id\n";
						else if(pcb->setPriority(priority) != EXIT_SUCCESS)
							message = "Invalid priority\n";
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
					while(gRTX->K_send_console_chars(ioLetter) == EXIT_ERROR);
					ioLetter = getMessage(MsgEnv::DISPLAY_ACK,gRTX);

					assure(ioLetter != NULL,"CCI:182 Failed to receive message after IO dealings!",__FILE__,__LINE__,__func__,true);
				}
				gRTX->K_release_processor();
			}	
		}
	}
}
