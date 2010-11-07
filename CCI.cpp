#include "CCI.h"

//REMOVE AFTER I/O IS IMPLEMENTED
#include <stdio.h>

CCI::CCI(RTX* rtx)
{
	_rtx = rtx;
	//wallClock = new WallClock();
	//_rtx.start();
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

		if(params > 0 && params < 4)
		{

			if(input[0] == "s")
			{
				if(params > 1)
					errMsg = "Too many parameters";
				cout << "sendMessage\n";
			}
			else if(input[0] == "ps")
			{
				if(params > 1)
					errMsg = "Too many parameters";
				cout << "processStatus\n";
			}
			else if(input[0] == "c")
			{
				string time[4];				
				if(params > 2)
					errMsg = "Too many parameters";
				else if(!parseString(input[1],time,':',3) != 3 || _rtx->wallClock->setTime(time) != EXIT_SUCCESS)
					errMsg = "Invalid time format";
			}
			else if(input[0] == "cd")
			{
				if(params > 1)
					errMsg = "Too many parameters";
				cout << "displayWallClock\n";
			}
			else if(input[0] == "ct")
			{
				if(params > 1)
					errMsg = "Too many parameters";
				cout << "hideWallClock\n";
			}
			else if(input[0] == "b")
			{
				if(params > 1)
					errMsg = "Too many parameters";
				cout << "displayTraceBuffers\n";
			}
			else if(input[0] == "t"){
				if(params > 1)
					errMsg = "Too many parameters";
				_rtx->signalHandler->handler(SIGINT);
			}
			else if(input[0] == "b")
			{
				if(params > 1)
					errMsg = "Too many parameters";
				cout << "displayTraceBuffers\n";
			}
			else if(input[0] == "n")
			{
				int pid,priority;
				PCB* pcb = NULL;
				if(strToInt(input[1],&priority) != EXIT_SUCCESS || strToInt(input[2],&pid) != EXIT_SUCCESS)
					errMsg = "invalid parameters";
				else if(_rtx->getPcb(pid,pcb) != EXIT_SUCCESS)
					errMsg = "Invalid process id";
				else if(pcb->setPriority(priority) != EXIT_SUCCESS)
					errMsg = "Invalid priority";
			}			
			else
			{
				errMsg = "Invalid Command Identifier: '" + command + "'";
				//cout << "1>" << input[0]  << "   2>" << input[1] << "   3>" << input[2] << "\n";
			}
		}
		else
			errMsg = "Invalid Command String";
		
		if(errMsg.length() > 0)
			cout << (errMsg + "\n");
	}	
}
