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
