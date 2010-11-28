#include "tests.h"
#include <sstream>

#define PRINT_Q_RESULTS 1
//#define VERBAL 1 //Whether the tests show output-> Turn to 0 for no output->

extern RTX* gRTX;

int testParser()
{
	int ret = EXIT_SUCCESS;
	string tokens[3];

	parseString("test parsing function  fails", tokens, ' ', 3);

	if (tokens[0] != "test" || tokens[1] != "parsing" || tokens[2] != "function")
		ret = EXIT_ERROR;
	
	return ret;
}

int testSignals()
{
	int ret = EXIT_SUCCESS;
/*
	try
	{
		gRTX->atomic(true);
		gRTX->atomic(false);
	}
	catch(int e)
	{
		ret = EXIT_ERROR;
	}
*/	
	return ret;
}

int testQueues()
{
	int ret = EXIT_SUCCESS;

	try
	{
		#if PRINT_Q_RESULTS == 1
		MsgEnv* test1 = new MsgEnv(); test1->setMsgType(1);
		MsgEnv* test2 = new MsgEnv(); test2->setMsgType(2);
		MsgEnv* test3 = new MsgEnv(); test3->setMsgType(3);
		MsgEnv* test4 = new MsgEnv(); test4->setMsgType(4);
		MsgEnv* test5 = new MsgEnv(); test5->setMsgType(5);
		MsgEnv* test6 = new MsgEnv(); test6->setMsgType(6);


		debugMsg("QUEUE TEST\n--------------------------\n\nInstantiate Queue...\n");

		Queue* q = new Queue(1);
		cout<<q->toString();
		debugMsg("\nTest isEmpty...");
		int test = q->isEmpty();
		debugMsg(((test==1) ? "YES" : "NO" ));
		debugMsg("\n");

//		debugMsg("\nTest Enqueue...\n");
//		q->enqueue((void**)(&test1));
//		cout<<q->toString();
//		
//		debugMsg("\nTest getLength...Length = ");
//		debugMsg(intToStr(q->get_length()));
//		debugMsg("\n");
//		
//		debugMsg("\nTest Dequeue...");
//		MsgEnv* deq = q->dequeue_MsgEnv();
//		debugMsg("Dequeued Value: ");
//		debugMsg(intToStr(deq->getMsgType()));
//		debugMsg("\n");
//		cout<<q->toString();
//		
//		debugMsg("\nPopulate Queue...\n");
//		q->enqueue((void**)(&test1));
//		q->enqueue((void**)(&test2));
//		q->enqueue((void**)(&test3));
//		q->enqueue((void**)(&test4));
//		cout<<q->toString();
//		debugMsg("Length = ");
//		debugMsg(intToStr((q->get_length())));
//		debugMsg("..........");
//		test = q->isEmpty();
//		debugMsg(((test==1) ? "EMPTY QUEUE" : "NOT EMPTY QUEUE" ));
//		debugMsg("\n");

//		debugMsg("\nTest contains...Contains 3?   ");
//		test = q->contains((void**)(&test3));
//		debugMsg(((test==1) ? "YES" : "NO" ));
//		debugMsg("\n");

//		debugMsg("\nTest contains...Contains 5?   ");
//		test = q->contains((void**)(&test5));
//		debugMsg(((test==1) ? "YES" : "NO" ));
//		debugMsg("\n");
//		
//		debugMsg("\nTest getQueueType...");
//		debugMsg(q->get_queueType());
//	
//		debugMsg("\n\nTest Replace...Replace 3 with 5\n");
//		debugMsg((q->replace((void**)(&test3), (void**)(&test5)) == 1?"PASS\n":"FAIL\n"));
//		cout<<q->toString();
//		
//		debugMsg("\n\nTest Replace...Replace 1 with 3\n");
//		debugMsg((q->replace((void**)(&test1), (void**)(&test3)) == 1?"PASS\n":"FAIL\n"));
//		cout<<q->toString();
//		
//		debugMsg("\n\nTest Replace...Replace 4 with 1\n");
//		debugMsg((q->replace((void**)(&test4), (void**)(&test1)) == 1?"PASS\n":"FAIL\n"));
//		cout<<q->toString();
//		
//		debugMsg("\nTest Bad Replace...Replace 6 with 5\n");
//		debugMsg((q->replace((void**)(&test6), (void**)(&test5)) == 1?"PASS\n":"FAIL\n"));
//		cout<<q->toString();
//		
//		debugMsg("\nTest Select...select 4\nSelected Value...");
//		MsgEnv* res = q->select(&test4);
//		if(res != NULL)
//			debugMsg(intToStr(res->getMsgType()));
//		else
//			debugMsg("Failed select");

//		debugMsg("\n\nTest Bad Select...select 6\nSelected Value...");
//		res = q->select(&test6);
//		if(res != NULL)
//			debugMsg(intToStr(res->getMsgType()));
//		else
//			debugMsg("Failed select");

//		debugMsg("\n\nTest Pluck...Pluck 2\nPlucked Value...");
//		res = q->pluck(&test2);
//		if(res != NULL)
//		{
//			debugMsg(intToStr(res->getMsgType()));
//			debugMsg("\n");
//		}
//		else
//			debugMsg("Failed pluck\n");
//		cout<<q->toString();
//		
//		debugMsg("\nTest Bad Pluck...Pluck 6\nPlucked Value...");
//		res = q->pluck(&test6);
//		if(res != NULL)
//		{
//			debugMsg(intToStr(res->getMsgType()));
//			debugMsg("\n");
//		}
//		else
//			debugMsg("Failed pluck\n");
//		cout<<q->toString();
		#endif
	
	
	
	debugMsg("Sorted Enqueues\n");
	MsgEnv* newMsg = new MsgEnv();
	newMsg->setMsgData( "TEST ENV" );
	newMsg->setTimeStamp( 190 );
	q->sortedEnqueue( (&newMsg));

	cout << q->toString();
	
	}
	catch(char* err){ret = EXIT_ERROR; }
	return ret;
}
