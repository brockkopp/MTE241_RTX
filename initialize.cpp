#include "debug.h"
#include "RTX.h"
#include "CCI.h"
#include <sys/mman.h>
#include <fcntl.h>
//#include <unistd.h>
#include "SignalHandler.h"
#include "tests.h"


#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#include <string.h>
#include <stdlib.h>
//
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>			// getpid() definition

#include <fcntl.h>

#include <sys/mman.h>
//#include <sys/wait.h>
#include <stdio.h>

//Globals
RTX* gRTX;
CCI* gCCI;

//Private method declarations
void doTests();
int initializeShmem();
int cleanupShmem();
int createInitTable(PcbInfo* initTable[]);

struct Shmem
{
	caddr_t rxPtr;
	caddr_t txPtr;
	char* 	rxFileName;
	char* 	txFileName;
	int 	rxId;
	int		txId;
	const static int 	bufferSize = 128;
} shmem;


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
	assure(initializeShmem() == EXIT_SUCCESS, "Shared memory failed to initialize", __FILE__, __LINE__, __func__, false);		//TRUE!!!!!!!!!!!
	//Initialize init table and assure initialization is successful
	assure(createInitTable(initTable) == EXIT_SUCCESS, "Init table failed to initialize", __FILE__, __LINE__, __func__, true);
	
	//Create and initialize rtx and its child members (schedling services etc)
	debugMsg("\n");
	
	gRTX = new RTX(initTable, sigHandler);
	debugMsg("\n");

	//Create keyboad thread
	if ((pidKB = fork()) == 0)
	{
		execl("./KB.out", myPid, (char*)NULL);

		//if the execution reaches here, the keyboard thread failed to initialize
		assure(false, "Keyboard helper process failed to initialize", __FILE__, __LINE__, __func__, true);
		exit(1);
	}
	if ((pidCRT = fork()) == 0)
	{
		execl("./CRT.out", myPid, (char*)NULL);

		//if the execution reaches here, the crt thread failed to initialize
		assure(false, "CRT helper process failed to initialize", __FILE__, __LINE__, __func__, true);
		exit(1);
	}
	//wait to assure that keyboard and crt initialize properly
	sleep(1);
	debugMsg("\n");

#if TESTS_MODE == 1
	doTests();
#endif

	//Initialize Tick Signals
	//ualarm(100,100);

	debugMsg("Type 'help' at any time to list possible CCI commands",0,2);	

	gCCI = new CCI();
	delete gCCI;

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
	debugMsg((sigNum == 0) ? "normally" : "UNEXPECTEDLY: " + intToStr(sigNum) ,0,1);	//SIGNUM 0 denotes manual exit from RTX primitive

	gRTX->atomic(true);

	//Cleanup shared memeory and assure that cleanup is successful
	assure(cleanupShmem() == EXIT_SUCCESS, "Shared memory cleanup failed", __FILE__, __LINE__, __func__, false);
	
	//Kill keyboard and wait until thread dies
	kill(pidKB,SIGKILL);
	wait();	

	//Kill crt and wait until thread dies
	kill(pidCRT,SIGKILL);
	wait();	

	//Cleanup rtx, including signal handler
	delete gRTX;
	delete gCCI;

	debugMsg("------------------------------------\n    RTX TERMINATED SUCCESSFULLY\n------------------------------------",1,2);	
	
	//Finally exit program
	exit(EXIT_SUCCESS);
}

int initializeShmem()
{
	cout << open((char *)"junkDemo", O_RDWR | O_CREAT | O_EXCL, (mode_t) 0755 );

	int result;

	shmem.rxFileName = (char *)"rxFile";
	shmem.rxFileName = (char *)"txFile";

	int success = 0;

	shmem.rxId = open(shmem.rxFileName, O_RDWR | O_CREAT | O_EXCL, (mode_t) 0755 );
	success += assure(shmem.rxId >= 0,"RX Shared memory file failed to initialize",__FILE__,__LINE__,__func__,false);

	result = ftruncate(shmem.rxId, shmem.bufferSize ); 
	success += assure(!result,"RX Shared memory file failed to truncate",__FILE__,__LINE__,__func__,false);

	shmem.txId = open(shmem.txFileName, O_RDWR | O_CREAT | O_EXCL, (mode_t) 0755 );
	success += assure(shmem.txId >= 0,"TX Shared memory file failed to initialize",__FILE__,__LINE__,__func__,false);

	result = ftruncate(shmem.txId, shmem.bufferSize ); 
	success += assure(!result,"TX Shared memory file failed to truncate",__FILE__,__LINE__,__func__,false);

	shmem.rxPtr = (char *)mmap((caddr_t) 0, shmem.bufferSize, PROT_READ | PROT_WRITE, MAP_SHARED, shmem.rxId, (off_t) 0);
	success += assure(shmem.rxPtr != MAP_FAILED,"RX memory map failed to initialize",__FILE__,__LINE__,__func__,false);

	shmem.txPtr = (char *)mmap((caddr_t) 0, shmem.bufferSize, PROT_READ | PROT_WRITE, MAP_SHARED, shmem.txId, (off_t) 0);
	success += assure(shmem.txPtr != MAP_FAILED,"TX memory map failed to initialize",__FILE__,__LINE__,__func__,false);

	if(success == 6)
		result = EXIT_SUCCESS;
	else
		result = EXIT_ERROR;

	return result;
}

int cleanupShmem()
{
	int ret = EXIT_SUCCESS;
	try
	{
		munmap(shmem.rxPtr,shmem.bufferSize);
		munmap(shmem.txPtr,shmem.bufferSize);

		close(shmem.rxId);    
		close(shmem.txId);

		unlink(shmem.rxFileName);
		unlink(shmem.txFileName);
	}
	catch(int e)
	{
		ret = EXIT_ERROR;
	}

	return ret;
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
