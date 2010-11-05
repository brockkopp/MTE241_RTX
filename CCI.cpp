#include "CCI.h"

//REMOVE AFTER I/O IS IMPLEMENTED
#include <stdio.h>

CCI::CCI()
{
	processCCI();
}

int CCI::parseString( string input, string output[], char token, int maxCount)
{
	unsigned int strPos = 0;
	unsigned int prevPos = 0;
	int tokenCnt = 0;

	//Search until finished string or output[] is full
	while(strPos <= input.length() && tokenCnt < maxCount)
	{
		//if token is found, or end of string
		if(input[strPos] == token || strPos == input.length())
		{
			//Store string (without token)
			output[tokenCnt] = input.substr(prevPos, strPos - prevPos);
			tokenCnt ++;
			//Loop until next non-token character in input string
			while(input[strPos++] == token)
			prevPos = strPos;
		}
		else
			strPos++;
	}
	return tokenCnt;
}

void CCI::processCCI()
{
	string command;
	string input[4];
	bool valid;

	while(true)
	{
		command = "";
		valid = true;
		
		while(command.length() == 0){
			cout << "<RTX$";
			cin >> command;
		}

		if(parseString( command, input, ' ', 4) <= 3){

			if(input[0] == "s")
			{
				cout << "sendMessage\n";
			}
			else if(input[0] == "ps")
			{
				cout << "processStatus\n";
			}
			else if(input[0] == "c")		//00:00:00
			{
				cout << "setWallClock\n";
			}
			else if(input[0] == "cd")
			{
				cout << "displayWallClock\n";
			}
			else if(input[0] == "ct")
			{
				cout << "hideWallClock\n";
			}
			else if(input[0] == "b")
			{
				cout << "displayTraceBuffers\n";
			}
			else if(input[0] == "t"){
				//rtx->signalHandler->handler(SIGINT);
				die(EXIT_SUCCESS);
			}
			else if(input[0] == "b")
			{
				cout << "displayTraceBuffers\n";
			}
			else if(input[0] == "n")
			{
				cout << "changeProcessPriority\n";		//new_pri  pid
			}			
			else
				cout << "fuck off and type something proper\n";
		}
		else
			valid = false;
		
		if(!valid)
			cout << "Invalid command";
	}	
}
