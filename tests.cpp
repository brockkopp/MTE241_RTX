#include "tests.h"

int testParser()
{
	int ret = EXIT_SUCCESS;
	string tokens[3];

	parseString("test parsing function  fails", tokens, ' ', 3);

	if (tokens[0] != "test" || tokens[1] != "parsing" || tokens[2] != "function")
		ret = EXIT_ERROR;
	
	return ret;
}


