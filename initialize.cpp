#include "debug.h"
#include "RTX.h"
#include "CCI.h"
#include <cstring>
#include "signal.h"
#include "SignalHandler.h"
#include "tests.h"

#define ANG_TEST 0

//Private method declarations
void doTests();
int inititalizeShmem();
int cleanupShmem();
int createInitTable(PcbInfo* initTable[]);

RTX* rtx;
CCI* cci;
caddr_t shemFiles[2];
int pidKB, pidCRT;
char* myPid;

int main(void)
{
	//Create init table
	PcbInfo* initTable[PROCESS_COUNT];

	myPid = (char*)intToStr(getpid()).c_str();

	debugMsg("------------------------------------\n           RTX INITIALIZED\n------------------------------------",1,2);	

	//Create and initialize signal handler
	//Signals are masked by default
	SignalHandler* sigHandler = new SignalHandler();

	//Create shared memory and assure that initialization is successful
	assure(inititalizeShmem() == EXIT_SUCCESS, "Shared memory failed to initialize", __FILE__, __LINE__, __func__, true);
	//Initialize init table and assure initialization is successful
	assure(createInitTable(initTable) == EXIT_SUCCESS, "Init table failed to initialize", __FILE__, __LINE__, __func__, true);
	
	//Create and initialize rtx and its child members (schedling services etc)
	debugMsg("\n");
	rtx = new RTX(initTable, sigHandler);
	debugMsg("\n");

	//Create keyboad thread
	if ((pidKB = fork()) == 0)
	{
		execl("./KB", myPid, (char*)NULL);

		//if the execution reaches here, the keyboard thread failed to initialize
		assure(false, "Keyboard helper process failed to initialize", __FILE__, __LINE__, __func__, true);
		exit(1);
	}
	if ((pidCRT = fork()) == 0)
	{
		execl("./CRT", myPid, (char*)NULL);

		//if the execution reaches here, the crt thread failed to initialize
		assure(false, "CRT helper process failed to initialize", __FILE__, __LINE__, __func__, true);
		exit(1);
	}
	//wait to assure that keyboard and crt initialize properly
	sleep(1);
	debugMsg("\n");
	
	//Unmask signals
	rtx->signalHandler->setSigMasked(false);

#if TESTS_MODE == 1
	doTests();
#endif

	//Initialize Tick Signals
	//ualarm(100,100);

	debugMsg("Type 'help' at any time to list possible CCI commands",0,2);	

	cci = new CCI(rtx);
	delete cci;

	//Signal cci init failed, program should not normally reach this point
	die(EXIT_ERROR);
}

void doTests()
{
	debugMsg("Testing...",1,1);
	debugMsg("\tParser Test:\t");    
	   debugMsg((testParser() == EXIT_SUCCESS) ? "Pass" : "Fail",0,1);
	debugMsg("\tSignal Test:\t");    
	   debugMsg("Not Implemented\n");//debugMsg((testParser() == EXIT_SUCCESS) ? "Pass" : "Fail",0,1);
	debugMsg("\tQueue Test: \t");    
	   debugMsg((testQueues() == EXIT_SUCCESS) ? "Pass" : "Fail",0,1);
	debugMsg("\tMessaging Test:\t"); 
	   debugMsg("Not Implemented\n");//debugMsg((testParser() == EXIT_SUCCESS) ? "Pass" : "Fail",0,1);
	debugMsg("\tAnother Test:\t");   
	   debugMsg("Not Implemented\n");//debugMsg((testParser() == EXIT_SUCCESS) ? "Pass" : "Fail",0,1);
	debugMsg("\tAnother Test:\t");   
	   debugMsg("Not Implemented\n",0,2);//debugMsg((testParser() == EXIT_SUCCESS) ? "Pass" : "Fail",0,1);
}

void die(int sigNum)
{
	debugMsg("Terminate command initiated ",2,0);
	debugMsg((sigNum == EXIT_SUCCESS) ? "normally" : "UNEXPECTEDLY: " + intToStr(sigNum) ,0,1);

	//Force mask all signals 
	rtx->signalHandler->setSigMasked(true);

	//Cleanup shared memeory and assure that cleanup is successful
	assure(cleanupShmem() == EXIT_SUCCESS, "Shared memory cleanup failed", __FILE__, __LINE__, __func__, false);
	
	//Kill keyboard and wait until thread dies
	kill(pidKB,SIGKILL);
	wait();	

	//Kill crt and wait until thread dies
	kill(pidCRT,SIGKILL);
	wait();	

	//Cleanup rtx, including signal handler
	delete rtx;
	delete cci;

	debugMsg("------------------------------------\n    RTX TERMINATED SUCCESSFULLY\n------------------------------------",1,2);	
	
	//Finally exit program
	exit(EXIT_SUCCESS);
}

int inititalizeShmem()
{
/*
	for(int i=0; i < 2; i++)
	{

		shemFile[i] = mmap((caddr_t) 0, 128, PROT_READ | PROT_WRITE, MAP_SHARED, fid, (off_t) 0);
		ensure(shmemFile[i] != NULL,"Shared memory file failed to init",__FILE__,__LINE__,true);
		  		
		
	}

	create file with permissions for owner rwx access only
		verify file created successfully
		truncate file to 128bytes to match the memory buffer
		//note: first file is rx_shared_mem_f, second file is                 tx_shared_mem_f

	}

	in_pid = fork keyboard process
	if(in_pid == 0)  //deal with child process

	{

	    //rx_shared_mem_f descriptor is an arg
		execl(./keyboard, keyboard args);           

	//should never reach here
		declare execl failed and terminate

	}

	sleep(1);    //give keyboard process time to start

	out_pid = fork crt process

	if(out_pid  == 0)  //deal with child process

	{
		execl(crt args); //tx_shared_mem_f descriptor is an arg
		//should never reach here
		declare execl failed and terminate execution

	}

	sleep(1);    //give crt process time to start

	//create shared memory buffers for two processes

	caddr_t rx_memmap_pt = mmap(keyboard args);

	if(memory mapping not successful)

	    cleanup and terminate execution

	caddr_t tx_memmap_pt = mmap(crt args);

	if(memory mapping not successful)

	    cleanup and terminate execution

	//create membuf pointers so RTX kernel may access the shared mem         rx_mem_buf = (inputbuf*) rx_memmap_pt
	    tx_mem_buf = (inputbuf*) tx_memmap_pt
	}
*/
	return EXIT_SUCCESS;
}

int cleanupShmem()
{
/*	int ret = EXIT_SUCCESS;
	try
	{
		munmap (rx_memmap_pt)        //unmap the memory buffers
		munmap (tx_memmap_pt)

		close(rx_shared_mem_f_id)    //close temporary files
		close(tx_shared_mem_f_id)

		unlink(rx_shared_mem_f)    //delete temporary files
		unlink(tx_shared_mem_f)
	}
	catch(Exception e)
	{
		ret = EXIT_ERROR;
	}
	return ret;
*/
	return -2;
	return EXIT_SUCCESS;
}

int createInitTable(PcbInfo* initTable[])
{	
	//Loop through each init table entry and allocate memory
	for(int i = 0; i <= PROCESS_COUNT; i++){
		//Do not throw error upon failure, use own validation
		initTable[i] = new(std::nothrow) PcbInfo();
		if (initTable[i] == NULL)				//#Do we need this???	
		{
			delete[] initTable;		
			return EXIT_ERROR;
		}
		
		//Initialize fields which are consistent across processes
		initTable[i]->processId = i;
		initTable[i]->stackSize = STACK_SIZE;
	}

//Kernel Processes
	initTable[0]->name =		"i_timing";	
	initTable[0]->priority =        0;
	initTable[0]->processType = 	0;
	initTable[0]->address = 	NULL;

	initTable[1]->name =		"i_kb";	
	initTable[1]->priority =        0;
	initTable[1]->processType = 	0;
	initTable[1]->address = 	NULL;

	initTable[2]->name =		"i_crt";	
	initTable[2]->priority =        0;
	initTable[2]->processType = 	0;
	initTable[2]->address = 	NULL;

	initTable[3]->name =		"null";	
	initTable[3]->priority =        3;
	initTable[3]->processType = 	0;
	initTable[3]->address = 	NULL;

//User Processes
	initTable[4]->name =		"user1";	
	initTable[4]->priority =        2;
	initTable[4]->processType = 	0;
	initTable[4]->address = 	NULL;

	initTable[5]->name =		"user2";	
	initTable[5]->priority =        2;
	initTable[5]->processType = 	0;
	initTable[5]->address = 	NULL;

	initTable[6]->name =		"user3";	
	initTable[6]->priority =        2;
	initTable[6]->processType = 	0;
	initTable[6]->address = 	NULL;
	
	return EXIT_SUCCESS;
}
