#include "tools.h"
#include "debug.h"

string intToStr(int num)
{
	stringstream temp;
	temp << num;
	return temp.str();
}

string intToStr(int num, unsigned int length)
{
	string ret = intToStr(num);
	while(ret.length() < length)
		ret = "0" + ret;
	return ret;
}

int strToInt(string input, int* output)
{
	int ret = EXIT_SUCCESS;	
	istringstream buffer(input);
	
	try
	{
		buffer >> *output;
	}
	catch (int e)
	{
		ret = EXIT_ERROR;
	}

	return ret;
}

int parseString( string input, string output[], char token, int maxCount)
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

	if(strPos < input.length())
		return -1;
	else
		return tokenCnt;
}

string getSigDesc(int sigNum)
{
	string ret;
	
	switch(sigNum)
	{
		case -1:	ret = "RTX Fatal Error"; break;			//Emulated signal defined within application
		case 1:		ret = "Terminal Hangup"; break;
		case 2:		ret = "Interrupt from keyboard"; break;
		case 3:		ret = "Quit from keyboard"; break;
		case 4:		ret = "Illegal Instruction"; break;
		case 5:		ret = "Trace/breakpoint trap"; break;
		case 6:		ret = "Abort signal"; break;
		case 7:		ret = "Emulator Trap"; break;
		case 8:		ret = "Floating point exception"; break;
		case 9:		ret = "Kill signal"; break;
		case 10:	ret = "Bus error (bad memory access)"; break;
		case 11:	ret = "Invalid memory reference"; break;
		case 12:	ret = "Bad argument to routine"; break;
		case 13:	ret = "Broken pipe: write to pipe with no readers"; break;
		case 14:	ret = "Timer signal from ualarm"; break;
		case 15:	ret = "Termination signal"; break; 
		case 30:	ret = "User-defined signal 1 (KB)"; break;
		case 31:	ret = "User-defined signal 2 (CRT)"; break;
		default:	ret = "Undefined signal"; break;
	}

	return ret + " (" + intToStr(sigNum) + ")";
}
