#include "debug.h"
#include "RTX.h"
#include "CCI.h"
#include "SignalHandler.h"
#include "Shmem.h"

#include "iprocesses.h"
#include "userProcesses.h"

#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

#define STACK_SIZE 		16384	//Stack size in bytes

//Globals
RTX* gRTX;

inputBuffer* gRxMemBuf;
inputBuffer* gTxMemBuf;

//Private method declarations
int initializeShmem();
int cleanupShmem();
int createInitTable(PcbInfo** initTable);

struct Shmem
{
	caddr_t rx_mmap_ptr;
	caddr_t tx_mmap_ptr;
	char* 	rxFileName;
	char* 	txFileName;
	int 	rxId;
	int		txId;
	const static int 	bufferSize = BUFSIZE;
} shmem;

int pidKB = 0, 
	pidCRT = 0, 
	pidRTX = 0;

int main(void)
{
	PcbInfo* initTable[PROCESS_COUNT];

	pidRTX = getpid();

	//Create and initialize signal handler
	//Signals are masked by default
	SignalHandler* sigHandler = new SignalHandler();
	sigHandler->setSigMasked(false);
	
	//Create shared memory and assure that initialization is successful
	assure(initializeShmem() == EXIT_SUCCESS, "Shared memory failed to initialize", __FILE__, __LINE__, __func__, true);

	//Initialize init table and assure initialization is successful
	assure(createInitTable(initTable) == EXIT_SUCCESS, "Init table failed to initialize", __FILE__, __LINE__, __func__, true);

	//Create and initialize rtx and its child members (schedling services etc)
	debugMsg("\n");

	//Create keyborad thread
	if ((pidKB = fork()) == 0)
	{
		execl("./KB.out", (char *)intToStr(pidRTX).c_str(), (char *)intToStr(shmem.rxId).c_str(), (char *)NULL);

		//if the execution reaches here, the keyboard thread failed to initialize
		assure(false, "Keyboard helper process failed to initialize", __FILE__, __LINE__, __func__, true);
		exit(1);
	}
	//Create CRT thread
	if ((pidCRT = fork()) == 0)
	{
		execl("./CRT.out", (char *)intToStr(pidRTX).c_str(), (char *)intToStr(shmem.txId).c_str(), (char *)NULL);
		//if the execution reaches here, the crt thread failed to initialize
		assure(false, "CRT helper process failed to initialize", __FILE__, __LINE__, __func__, true);
		exit(1);
	}

	//wait to assure that keyboard and crt initialize properly
	sleep(1);
	debugMsg("\n");
	
	debugMsg("Type help at any time to list possible CCI commands",0,1);	

	gRTX = new RTX(initTable, sigHandler);

	//Start scheduler. Put the first process onto the CPU
	gRTX->start_execution();

	//	Signal cci init failed, program should not normally reach this point

	die(-1);
}

void die(int sigNum)
{
	debugMsg("Terminate command initiated ",2,0);
	debugMsg((sigNum == 0) ? "normally" : "UNEXPECTEDLY: " + getSigDesc(sigNum) ,0,1);	//SIGNUM 0 denotes manual exit from RTX primitive
	
	ualarm(0,0);	//Disable alarm while exiting
	assure(cleanupShmem() == EXIT_SUCCESS, "Shared memory cleanup failed (init)", __FILE__, __LINE__, __func__, false);

	//Cleanup rtx, including signal handler
	try
	{
		delete gRTX;
	}
	catch(int e)
	{
		debugMsg("Global variable cleanup failed",0,1);
	}

	//Kill KB and CRT child processes
	if(pidCRT != 0)
	{
		kill(pidCRT,SIGKILL);
		waitpid(pidCRT,NULL,0);	
		debugMsg("CRT process terminated",1,1);
	}
	if(pidKB != 0)
	{
		kill(pidKB,SIGKILL);
		waitpid(pidKB,NULL,0);	
	    debugMsg("KB  process terminated",0,1);
	}


	//Cleanup shared memeory and assure that cleanup is successful
	assure(cleanupShmem() == EXIT_SUCCESS, "Shared memory cleanup failed", __FILE__, __LINE__, __func__, true);

	debugMsg("------------------------------------\n    RTX TERMINATED SUCCESSFULLY\n------------------------------------",1,2);	
	
	//Finally exit program
	exit(EXIT_SUCCESS);
}

int initializeShmem()
{
	int ret = EXIT_SUCCESS;		//Default returned value
	int result;					//Temporarily stores return values
	int fail = 0;			//Keeps track of whether function was successful

	//Initialize files names. Files will be created in execution directory
	shmem.rxFileName = (char *)"rx.buf";
	shmem.txFileName = (char *)"tx.buf";

	//Create RX buffer file
	shmem.rxId = open(shmem.rxFileName, O_RDWR | O_CREAT | O_EXCL, (mode_t) 0755 );
	fail += assure(shmem.rxId != -1,"RX Shared memory file failed to initialize",__FILE__,__LINE__,__func__,false) ? 0 : 1;
	
	//Truncate RX buffer file to buffer size
	result = ftruncate(shmem.rxId, shmem.bufferSize ); 
	fail += assure(result == 0,"RX Shared memory file failed to truncate",__FILE__,__LINE__,__func__,false) ? 0 : 1;

	//Create TX buffer file
	shmem.txId = open(shmem.txFileName, O_RDWR | O_CREAT | O_EXCL, (mode_t) 0755 );
	fail += assure(shmem.txId != -1,"TX Shared memory file failed to initialize",__FILE__,__LINE__,__func__,false) ? 0 : 1;

	//Truncate TX buffer file to buffer size
	result = ftruncate(shmem.txId, shmem.bufferSize ); 
	fail += assure(result == 0,"TX Shared memory file failed to truncate",__FILE__,__LINE__,__func__,false) ? 0 : 1;

	//Create RX buffer association
	shmem.rx_mmap_ptr = (char *)mmap((caddr_t) 0, shmem.bufferSize, PROT_READ | PROT_WRITE, MAP_SHARED, shmem.rxId, (off_t) 0);
	fail += assure(shmem.rx_mmap_ptr != MAP_FAILED,"RX memory map failed to initialize",__FILE__,__LINE__,__func__,false) ? 0 : 1;

	//Create TX buffer association
	shmem.tx_mmap_ptr = (char *)mmap((caddr_t) 0, shmem.bufferSize, PROT_READ | PROT_WRITE, MAP_SHARED, shmem.txId, (off_t) 0);
	fail += assure(shmem.tx_mmap_ptr != MAP_FAILED,"TX memory map failed to initialize",__FILE__,__LINE__,__func__,false) ? 0 : 1;

	gRxMemBuf = NULL;
	gTxMemBuf = NULL;
	
	//Assure that all functions returned success
	if(fail == 0)
	{
		debugMsg("Shared Memory Initialization Successful");
		//Create pointers to communicate with shared memory
		gRxMemBuf = (inputBuffer*)shmem.rx_mmap_ptr;
		gTxMemBuf = (inputBuffer*)shmem.tx_mmap_ptr;
	}
	else
		ret = EXIT_ERROR;

	return ret;
}

int cleanupShmem()
{
	int ret = EXIT_SUCCESS;		//Default returned value

	try
	{
		munmap(shmem.rx_mmap_ptr,shmem.bufferSize);
		munmap(shmem.tx_mmap_ptr,shmem.bufferSize);

		close(shmem.rxId);    
		close(shmem.txId);

		unlink(shmem.rxFileName);
		unlink(shmem.txFileName);
	}
	catch(int e)
	{
		debugMsg("Shared memory cleanup failed (Error: " + intToStr(e) + ")");
		ret = EXIT_ERROR;
	}

	return ret;
}

int createInitTable(PcbInfo** initTable)
{	
	int ret = EXIT_SUCCESS;

	try	//Assure init table is allocated successfully
	{
		//Loop through each init table entry and allocate memory
		for(int i = 0; i <= PROCESS_COUNT; i++){
			//Do not throw error upon failure, use own validation
			initTable[i] = new PcbInfo();

			//Initialize fields which are consistent across processes
			initTable[i]->processId = i;
			initTable[i]->stackSize = STACK_SIZE;
		}

		initTable[0]->name =					"i_timing";	
		initTable[0]->priority =    	0;
		initTable[0]->processType = 	PROCESS_I;
		initTable[0]->address = 			&(i_timing_process);

		initTable[1]->name =					"i_kb";	
		initTable[1]->priority =    	0;
		initTable[1]->processType = 	PROCESS_I;
		initTable[1]->address = 			&(i_keyboard_handler);

		initTable[2]->name =					"i_crt";	
		initTable[2]->priority =    	0;
		initTable[2]->processType = 	PROCESS_I;
		initTable[2]->address = 			&(i_crt_handler);

		initTable[3]->name =					"null_proc";	
		initTable[3]->priority =    	3;
		initTable[3]->processType = 	PROCESS_N;
		initTable[3]->address = 			&(RTX::null_proc);

		initTable[4]->name =					"userA";	
		initTable[4]->priority =    	2;
		initTable[4]->processType = 	PROCESS_U;
		initTable[4]->address = 			&(userProcessA);

		initTable[5]->name =					"userB";	
		initTable[5]->priority =    	2;
		initTable[5]->processType = 	PROCESS_U;
		initTable[5]->address = 			&(userProcessB);

		initTable[6]->name =					"userC";	
		initTable[6]->priority =    	2;
		initTable[6]->processType = 	PROCESS_U;
		initTable[6]->address = 			&(userProcessC);
		
		initTable[7]->name =					"CCI";	
		initTable[7]->priority =    	2;
		initTable[7]->processType = 	PROCESS_K;
		initTable[7]->address = 			&(processCCI);

	}
	catch(int e)
	{
		delete[] initTable;
		ret = EXIT_ERROR;
	}
	
	return ret;
}
