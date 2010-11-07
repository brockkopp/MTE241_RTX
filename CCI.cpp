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
	string input[4];
	string errMsg;
	int params;

	while(true)
	{
		command = "";
		errMsg = "";
		
		while(command.length() == 0){
			cout << "<RTX$";
			cin >> command;
		}
		
		params = parseString( command, input, ' ', 4);
		
		if(params <= 3){

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
			else if(input[0] == "c")		//00:00:00
			{
				if(params > 2)
					errMsg = "Too many parameters";
				
				cout << "setWallClock\n";
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
				//rtx->signalHandler->handler(SIGINT);
				die(EXIT_SUCCESS);
			}
			else if(input[0] == "b")
			{
				if(params > 1)
					errMsg = "Too many parameters";
				cout << "displayTraceBuffers\n";
			}
			else if(input[0] == "n")
			{

				cout << "changeProcessPriority\n";		//new_pri  pid
			}			
			else
				errMsg = "Invalid Command";
		}
		else
			errMsg = "Invalid Command";
		
		if(errMsg.length() > 0)
			cout << (errMsg + "\n");
	}	
}
