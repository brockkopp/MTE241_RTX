#include "CCI.h"
extern RTX* gRTX;

//REMOVE AFTER I/O IS IMPLEMENTED
#include <stdio.h>

CCI::CCI()
{
	wallClock = new WallClock(100000);
	userInputs = new Queue(Queue::STRING);
	//ualarm(100000,100000);
}

CCI::~CCI()
{
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
			cout << ">RTX$ ";
			getline(cin,command);
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
					MsgEnv* myEnv = gRTX->K_request_msg_env();
					myEnv->setDestPid(USER_PROC_A);
					//Set other appropriate fields
					if(gRTX->K_send_message(USER_PROC_A, myEnv) != EXIT_SUCCESS)
						errMsg = "Message failed to send";					
				}
			}
			else if(input[0] == "ps")	//to complete
			{
				if(params > 1)
					errMsg = "Too many parameters for 'Display Process Status' command";
				else
					cout << "processStatus\n";
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
					MsgEnv* myEnv = gRTX->K_request_msg_env();
					if(gRTX->K_get_trace_buffers(myEnv) != EXIT_SUCCESS)
						errMsg = "Failed to retrieve message buffers";
					else 
						gRTX->K_send_console_chars(myEnv);
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
				else if(pcb->set_priority(priority) != EXIT_SUCCESS)
					errMsg = "Invalid priority";
			}
			else if(input[0] == "help")	//remove for demo
			{
				if(params > 1)
					errMsg = "Too many parameters for 'Help' command";
				else
				{
					cout << "\nRTX Commands:\n";
					cout << "\tSend Message            s\n";
					cout << "\tProcess Status          ps\n";
					cout << "\tSet Wall Clock          c hh:mm:ss\n";
					cout << "\tDisplay Wall Clock      cd\n";
					cout << "\tHide Wall Clock         ct\n";
					cout << "\tDisplay Message Trace   b\n";
					cout << "\tTerminate               t\n";
					cout << "\tChange Priority         n pri pid\n\n";
				}
			}			
			else
				errMsg = "Invalid Command Identifier: '" + input[0] + "'";
		}
		else
			errMsg = "Invalid Command String";
		
		if(errMsg.length() > 0)
			cout << ("\t" + errMsg + "\n");
	}	

	return EXIT_ERROR;
}
