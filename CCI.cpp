#include "CCI.h"
extern RTX* gRTX;
extern inputBuffer* gRxMemBuf;
extern CCI* gCCI;

//REMOVE AFTER I/O IS IMPLEMENTED
#define IO 1
#include <stdio.h>

CCI::CCI()
{
	wallClock = new WallClock(100000);
	userInputs = new Queue(Queue::STRING);
	
	ioLetter = gRTX->K_request_msg_env();
	
	//ualarm(TICK_TIME,TICK_TIME);
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
	string errMsg;
	int params;
	
	while(true)
	{
		command = "";
		input[0] = input[1] = input[2] = "";
		errMsg = "";		

		do
		{
			while(gRTX->K_send_console_chars(NULL) == EXIT_ERROR);
			
//			MsgEnv* testMsg = new MsgEnv();
//			testMsg->setMsgType(MsgEnv::TO_CRT);
//			testMsg->setMsgData(">RTX$1\n>RTX$2\n>RTX$3\n>RTX$4\n");
//			while(gRTX->displayText(testMsg) == EXIT_ERROR);
			
//			if(IO) cout << ">RTX$ ";
//			messageEnvIO->setMsgData(">RTX$ ");
//			while(gRTX->K_send_console_chars(messageEnvIO) == EXIT_ERROR);
//			messageEnvIO = gRTX->K_receive_message():
			
			
			//if(IO) getline(cin,command);
			
			ioLetter->setMsgData("");
			while(gRTX->K_get_console_chars(ioLetter) == EXIT_ERROR)
				usleep(1000000);
				
			command = *(gCCI->userInputs->dequeue_string());
						
			//ioLetter = gRTX->K_receive_message();
			//command = ioLetter->getMsgData();
			
			//cout<<"CCI repeat command: "<<command;
						
		}
		while(command.length() == 0);

		params = parseString( command, input, ' ', 3);

		if(params >= 1 && params <= 3)
		{
			if(input[0] == "s")		//to complete
			{
				if(params > 1)
					errMsg = "Too many parameters for 'Send Message' command";
				else
				{
					cout<<"send message\n";
//					MsgEnv* myEnv = gRTX->K_request_msg_env();
//					myEnv->setDestPid(USER_PROC_A);
//					myEnv->setMsgType(MsgEnv::WAKE_UP);
//					myEnv->setMsgData("");
//					
//					if(gRTX->K_send_message(USER_PROC_A, myEnv) != EXIT_SUCCESS)
//						errMsg = "Message failed to send";					
				}
			}
			else if(input[0] == "ps")
			{
				if(params > 1)
					errMsg = "Too many parameters for 'Display Process Status' command";
				else
				{
					cout<<"request process status\n";
//					if( gRTX->K_request_process_status(ioLetter) == EXIT_SUCCESS )
//						ioLetter = gRTX->K_receive_message();
//					else
//						ioLetter->setMsgData("Request Process Status Failed\n");

//					while(gRTX->K_send_console_chars(ioLetter) != EXIT_SUCCESS);
//					ioLetter = gRTX->K_receive_message();
				}
			}
			else if(input[0] == "c")
			{
				string time[3];				
				if(params > 2)
					errMsg = "Too many parameters for 'Set Clock' command";
				else if(parseString(input[1],time,':',3) != 3 || wallClock->setTime(time) != EXIT_SUCCESS)
					errMsg = "Invalid time format";
			}
			else if(input[0] == "cd")
			{
				if(params > 1)
					errMsg = "Too many parameters for 'Display Clock' command";
				else
					wallClock->setDisplayed(true);
			}
			else if(input[0] == "ct")
			{
				if(params > 1)
					errMsg = "Too many parameters for 'Hide Clock' command";
				else
					wallClock->setDisplayed(false);
			}
			else if(input[0] == "b")	//to complete
			{
				if(params > 1)
					errMsg = "Too many parameters for 'Display Msg Buffers' command";
				else
				{
					cout<<"get trace buffers\n";
//					if( gRTX->K_get_trace_buffers(ioLetter) == EXIT_SUCCESS )
//						ioLetter = gRTX->K_receive_message();
//					else
//						ioLetter->setMsgData("Display Trace Buffers Failed\n");

//					while(gRTX->K_send_console_chars(ioLetter) != EXIT_SUCCESS);
//					ioLetter = gRTX->K_receive_message();
				}
			}
			else if(input[0] == "t")
			{
				if(params > 1)
					errMsg = "Too many parameters for 'Terminate' command";
				else
					kill(getpid(),SIGINT);
			}
			else if(input[0] == "n")
			{
				int pid,priority;
				PCB* pcb = NULL;
				if(strToInt(input[1],&priority) != EXIT_SUCCESS || strToInt(input[2],&pid) != EXIT_SUCCESS)
					errMsg = "invalid parameters";
				else if(gRTX->getPcb(pid,&pcb) != EXIT_SUCCESS)
					errMsg = "Invalid process id";
				else if(pcb->setPriority(priority) != EXIT_SUCCESS)
					errMsg = "Invalid priority";
			}
			else if(input[0] == "scc")
			{
				gRTX->K_send_console_chars(NULL);
			}
			else if(input[0] == "help")	//remove for demo
			{
				if(params > 1)
					errMsg = "Too many parameters for 'Help' command";
				else
				{
					string output;
					output =  "\nRTX Commands:\n";
					output += "\tSend Message            s\n";
					output += "\tProcess Status          ps\n";
					output += "\tSet Wall Clock          c hh:mm:ss\n";
					output += "\tDisplay Wall Clock      cd\n";
					output += "\tHide Wall Clock         ct\n";
					output += "\tDisplay Message Trace   b\n";
					output += "\tTerminate               t\n";
					output += "\tChange Priority         n pri pid\n\n";
					//debugMsg(output);
					cout<<output;
				}
			}			
			else
				errMsg = "Invalid Command Identifier: '" + input[0] + "'";
		}
		else
			errMsg = "Invalid Command String";
		
		if(errMsg.length() > 0)
		{
			cout<<"\t"<<errMsg<<"\n";
//			//debugMsg("\t" + errMsg + "\n");
//			ioLetter->setMsgData("\t" + errMsg + "\n");
//			while(gRTX->K_send_console_chars(ioLetter) != EXIT_SUCCESS);
//			ioLetter = gRTX->K_receive_message();
		}
	}	

	return EXIT_ERROR;
}
