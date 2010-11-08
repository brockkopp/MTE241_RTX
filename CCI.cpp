#include "CCI.h"

//REMOVE AFTER I/O IS IMPLEMENTED
#include <stdio.h>

CCI::CCI(RTX* rtx)
{
	_rtx = rtx;
	wallClock = new WallClock();
	processCCI();
}

void CCI::processCCI()
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
					errMsg = "Too many parameters";
				else
				{
					MsgEnv* myEnv = _rtx->K_request_msg_env();
					myEnv->setDestPid(USER_PROC_A);
					//Set other appropriate fields
					if(_rtx->K_send_message(USER_PROC_A, myEnv) != EXIT_SUCCESS)
						errMsg = "Message failed to send";
					
				}
			}
			else if(input[0] == "ps")	//to complete
			{
				if(params > 1)
					errMsg = "Too many parameters";
				else
					cout << "processStatus\n";
			}
			else if(input[0] == "c")
			{
				string time[4];				
				if(params > 2)
					errMsg = "Too many parameters";
				else if(!parseString(input[1],time,':',3) != 3 || wallClock->setTime(time) != EXIT_SUCCESS)
					errMsg = "Invalid time format";
			}
			else if(input[0] == "cd")
			{
				if(params > 1)
					errMsg = "Too many parameters";
				else
					wallClock->setDisplayed(true);
			}
			else if(input[0] == "ct")
			{
				if(params > 1)
					errMsg = "Too many parameters";
				else
					wallClock->setDisplayed(false);
			}
			else if(input[0] == "b")	//to complete
			{
				if(params > 1)
					errMsg = "Too many parameters";
				else
				{
					MsgEnv* myEnv = _rtx->K_request_msg_env();
					if(_rtx->K_get_trace_buffers(myEnv) != EXIT_SUCCESS)
						errMsg = "Failed to retrieve message buffers";
					else 
						_rtx->K_send_console_chars(myEnv);
				}
			}
			else if(input[0] == "t")
			{
				if(params > 1)
					errMsg = "Too many parameters";
				else
					_rtx->signalHandler->handler(SIGINT);
			}
			else if(input[0] == "n")
			{
				int pid,priority;
				PCB* pcb = NULL;
				if(strToInt(input[1],&priority) != EXIT_SUCCESS || strToInt(input[2],&pid) != EXIT_SUCCESS)
					errMsg = "invalid parameters";
				else if(_rtx->getPcb(pid,&pcb) != EXIT_SUCCESS)
					errMsg = "Invalid process id";
				else if(pcb->set_priority(priority) != EXIT_SUCCESS)
					errMsg = "Invalid priority";
			}
			else if(input[0] == "help")	//remove for demo
			{
				if(params > 1)
					errMsg = "Too many parameters";
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
}
