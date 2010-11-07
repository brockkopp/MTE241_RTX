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

int testQueues()
{
	int ret = EXIT_SUCCESS;
	
//	int testdata1 = 1;
//	int testdata2 = 2;
//	int testdata3 = 3;
//	int testdata4 = 4;

//	debugMsg("Instantiating queue!\n");
//	Queue q("int");

//	debugMsg("Enqueue\n");
//	q.enqueue( testdata1 );

//	debugMsg("Test length()\n");
//	int l = q.get_length();
//	debugMsg("Output of length(). Should = 1: " + l + "\n");

//	debugMsg("Dequeue. Also tests the 'edgecase' of dq when length == 1";
//	int* deq = q.dequeue();

//	debugMsg("Value of dequeued node. Should = 1:" + *deq + "\n";


//	q.enqueue( testdata2 );
//	q.enqueue( testdata3 );
//	q.enqueue( testdata4 );

//	q.dequeue( );
//	q.dequeue( );
//	q.dequeue( );
//	q.dequeue( );

	
	return ret;
}


