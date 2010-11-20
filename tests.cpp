#include "tests.h"
#include <sstream>

#define PRINT_Q_RESULTS 0

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
		int test1 = 1;
		int test2 = 2;
		int test3 = 3;
		int test4 = 4;
		int test5 = 5;
		int test6 = 6;

		debugMsg("QUEUE TEST\n--------------------------\n\nInstantiate Queue...\n");

		Queue q("int");	
		q.printIntQueue();
		debugMsg("\nTest isEmpty...");
		int test = q.isEmpty();
		debugMsg(((test==1) ? "YES" : "NO" ));
		debugMsg("\n");

		debugMsg("\nTest Enqueue...\n");
		q.enqueue(&test1);
		q.printIntQueue();

		debugMsg("\nTest getLength...Length = ");
		debugMsg(intToStr(q.get_length()));
		debugMsg("\n");
		
		debugMsg("\nTest Dequeue...");
		int* deq = q.dequeue_int();
		debugMsg("Dequeued Value: ");
		debugMsg(intToStr(*deq));
		debugMsg("\n");
		q.printIntQueue();

		debugMsg("\nPopulate Queue...");
		q.enqueue(&test1);
		q.enqueue(&test2);
		q.enqueue(&test3);
		q.enqueue(&test4);
		q.printIntQueue();
		debugMsg("Length = ");
		debugMsg(intToStr((q.get_length())));
		debugMsg(".......");
		test = q.isEmpty();
		debugMsg(((test==1) ? "EMPTY QUEUE" : "NOT EMPTY QUEUE" ));
		debugMsg("\n");

		debugMsg("\nTest contains...Contains 3?   ");
		test = q.contains(&test3);
		debugMsg(((test==1) ? "YES" : "NO" ));
		debugMsg("\n");

		debugMsg("\nTest contains...Contains 5?   ");
		test = q.contains(&test5);
		debugMsg(((test==1) ? "YES" : "NO" ));
		debugMsg("\n");
		
		debugMsg("\nTest getQueueType...");
		debugMsg(q.get_queueType());
	
		debugMsg("\n\nTest Replace...Replace 3 with 5\n");
		debugMsg((q.replace(&test3, &test5) == 1?"PASS\n":"FAIL\n"));
		q.printIntQueue();

		debugMsg("\nTest Bad Replace...Replace 6 with 5\n");
		debugMsg((q.replace(&test6, &test5) == 1?"PASS\n":"FAIL\n"));
		q.printIntQueue();

		debugMsg("\nTest Select...select 4\nSelected Value...");
		int* res = q.select(&test4);
		if(res != NULL)
			debugMsg(intToStr(*res));
		else
			debugMsg("Failed select");

		debugMsg("\n\nTest Bad Select...select 6\nSelected Value...");
		res = q.select(&test6);
		if(res != NULL)
			debugMsg(intToStr(*res));
		else
			debugMsg("Failed select");

		debugMsg("\n\nTest Pluck...Pluck 2\nPlucked Value...");
		res = q.pluck(&test2);
		if(res != NULL)
		{
			debugMsg(intToStr(*res));
			debugMsg("\n");
		}
		else
			debugMsg("Failed pluck\n");
		q.printIntQueue();

		debugMsg("\nTest Bad Pluck...Pluck 6\nPlucked Value...");
		res = q.pluck(&test6);
		if(res != NULL)
		{
			debugMsg(intToStr(*res));
			debugMsg("\n");
		}
		else
			debugMsg("Failed pluck\n");
		q.printIntQueue();
		#endif
	}
	catch(char* err){ret = EXIT_ERROR; }
	return ret;
}

int testScheduler( Scheduler* S )
{
	const int VERBAL = 1; // Turn this to zero to turn off output.

	int ret = EXIT_SUCCESS;
	
	debugMsg("\n&&&&&&&&&&&&&&&&&&&&&&&&\n");

/*--- Functions to test---*/			
//			void release_processor( );   
//			int change_priority( PCB * target, int newPriority );
//			int process_switch( );

//			//Place new process on ready queue
//		  int add_ready_process( PCB * target );
//			int block_process (PCB * target, int reason );
//			int unblock_process( PCB * target );
//			
//			//Returns if a process is currently blocked on envelope
//			int is_blocked( PCB * target );
//		
//			PCB* get_current_process();

	//Check how many processes are on the queues
	debugMsg("\nerror in context_Restore ::: lib/Context.cpp:35\n");

	debugMsg("\nI think the invalid memory reference is caused by the get_current_process call as tehre is not yet a current process.\n");
	//TEST process_switch()
if (VERBAL) debugMsg("current_process:"+ S->get_current_process()->get_name() + "\n");
//	S->process_switch();
	
	return ret;
}


