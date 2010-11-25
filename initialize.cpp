#include "debug.h"
#include "RTX.h"
#include "CCI.h"
#include "SignalHandler.h"
#include "tests.h"
#include "Shmem.h"
#include "lib/Mailbox.h"

/* Not sure it'salright to include .cpp's need to review this --Karl */
//#include "iprocesses.cpp"
#include "userProcesses.h"

#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

#define STACK_SIZE 		16372	//Stack size in bytes


//Globals
RTX* gRTX;
Queue* gUserInputs;

int gRunTime = 0;
inputBuffer* gRxMemBuf;
inputBuffer* gTxMemBuf;

//Private method declarations
void doTests();
int initializeShmem();
int cleanupShmem();
int createInitTable(PcbInfo* initTable[]);

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
	
PcbInfo **initTable;

int main(void)
{
	initTable = new PcbInfo *[7];

	pidRTX = getpid();

	debugMsg("------------------------------------\n           RTX INITIALIZED\n------------------------------------",1,2);	

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

#if TESTS_MODE == 1
	//doTests();
#endif
	gRTX = new RTX(initTable, sigHandler);
	//processCCI();		//TESTING ONLY -- SHOULD BE PROCESS

	//Start scheduler. Put the first process onto the CPU
	gRTX->start_execution();

//	Signal cci init failed, program should not normally reach this point
	//assure(processCCI() == EXIT_SUCCESS,"CCI exited unexpectedly",__FILE__,__LINE__,__func__,true);
	die(-1);
}

void doTests()
{
	/*
	debugMsg("Testing...",1,1);
	debugMsg("\tParser Test:\t");    
	   debugMsg((testParser() == EXIT_SUCCESS) ? "Pass" : "Fail",0,1);
	debugMsg("\tSignal Test:\t");    
	   debugMsg((testSignals() == EXIT_SUCCESS) ? "Pass" : "Fail",0,1);
	debugMsg("\tQueue Test: \t");    
	   debugMsg((testQueues() == EXIT_SUCCESS) ? "Pass" : "Fail",0,1);
	debugMsg("\tMessaging Test:\t"); 
	   debugMsg("Not Implemented\n");
//	debugMsg("\tScheduler Test:\t");   
//	   debugMsg((testScheduler( gRTX->getScheduler() ) == EXIT_SUCCESS) ? "Pass" : "Fail",0,1);
	debugMsg("\tPQ Test:\t");   
	   debugMsg((testPQ() == EXIT_SUCCESS) ? "Pass" : "Fail",0,1);
	debugMsg("\tAnother Test:\t");   
	   debugMsg("Not Implemented\n",0,2);//debugMsg((testParser() == EXIT_SUCCESS) ? "Pass" : "Fail",0,1);
 	debugMsg("\tAnother Test:\t");   
	   debugMsg("Not Implemented\n",0,2);//debugMsg((testParser() == EXIT_SUCCESS) ? "Pass" : "Fail",0,1);
 	debugMsg("\tAnother Test:\t");   
	   debugMsg("Not Implemented\n",0,2);//debugMsg((testParser() == EXIT_SUCCESS) ? "Pass" : "Fail",0,1);

	  */ 
//*******************************************************ERIC TEST****************************************************************	
	
	debugMsg("ERIC TEST\n---------\n");
	
	PCB* tempPCB;
	assure((tempPCB = gRTX->getCurrentPcb()) != NULL,"Failed to retrieve current PCB",__FILE__,__LINE__,__func__,false); //ERic
	
	MsgEnv* msg = gRTX->K_request_msg_env();
	debugMsg("msg allocated\n");
	msg->setMsgData("test 1");
	debugMsg("msg data added\n");
	msg->setMsgType(5);
	debugMsg("msg type added\n");
	msg->setTimeStamp(10);
	debugMsg("msg time stamp added\n");
	gRTX->K_send_message(6,msg);
	debugMsg("msg sent\n");
	msg = NULL;

	PCB* testPtr;
	gRTX->getPcb(6,&testPtr);
	debugMsg("receiver PCB retrieved\n");
	msg = gRTX->K_receive_message();
	debugMsg("mail retrieved\n");
	if (msg != NULL)
	{
	debugMsg("origin PID: "+intToStr(msg->getOriginPid())+"\n");
	debugMsg("dest PID: "+intToStr(msg->getDestPid())+"\n");
	debugMsg("Msg Type: "+intToStr(msg->getMsgType())+"\n");
	debugMsg("Time Stamp: "+intToStr(msg->getTimeStamp())+"\n");
	debugMsg("MsgData: "+msg->getMsgData()+"\n");
	}
	debugMsg("releasing env..\n");
	debugMsg("Successful?: "+intToStr(gRTX->K_release_msg_env(msg))+"\n");
	
	msg = gRTX->K_request_msg_env();
	debugMsg("msg allocated\n");
	debugMsg("printing trace buffers...\n");
	gRTX->K_get_trace_buffers(msg);
	gRTX->K_send_console_chars(msg);
	
	
	debugMsg("ERIC TEST END\n-------------\n");	
//********************************************************ERIC TEST END**********************************************************	 


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

void a()
{
	cout << "\nA\n\n";
//	gRTX->_jmpList[1]->restore_context();
	gRTX->K_release_processor();
}
void b()
{
	cout << "\nB\n\n";
//	longjmp(gRTX->_jmpList[2]->_buffer,1);
	gRTX->K_release_processor();
}
void c()
{
	cout << "\nC\n\n";
//	longjmp(gRTX->_jmpList[3]->_buffer,1);
	gRTX->K_release_processor();
}
void d()
{
	cout << "\ntest Null proc!\n\n";
	sleep(2);
	gRTX->K_release_processor();
}
void e()
{
while (true) {	cout << "_readyProcs: " << gRTX->getScheduler()->_readyProcs->toString() << "\n";
	cout << "\nuserA\n\n";
	gRTX->K_release_processor();
	cout << "\nuserA here #2!\n";
}
}
void f()
{
while (true) {	cout << "_readyProcs: " << gRTX->getScheduler()->_readyProcs->toString() << "\n";
	cout << "\nuserB\n\n";
	gRTX->K_release_processor();
	cout << "\nuserB here #2!\n";
	
	cout << "UNBLOCK CCI \n";
	cout << "_blockedEnv: " << gRTX->getScheduler()->_blockedEnv->toString() << "\n";
	cout << "blocked_proc_state: " << gRTX->_pcbList[7]->getState() << "\n";
	gRTX->getScheduler()->unblock_process( gRTX->_pcbList[7] );//Unblock CCI
}
}
void g()
{
while (true) {
	cout << "_readyProcs: " << gRTX->getScheduler()->_readyProcs->toString() << "\n";
	cout << "\nuserC\n\n";
	gRTX->K_release_processor();
	cout << "\nuserC here #2!\n";
	
	cout << "CCI_proc_state: " << gRTX->_pcbList[7]->getState() << "\n";
	cout << "About to change priority of CCI\n";
	gRTX->_scheduler->change_priority(gRTX->_pcbList[7], 0);
	
}
}
void test_CCI()
{

/* Acts as a tst cci process */

//Note that this is a great place to place scheduler-specific fxns to test them.
while (true) {
	gRTX->atomic( 1 );
	cout << "_readyProcs: " << gRTX->getScheduler()->_readyProcs->toString() << "\n";
	
	cout << "_blockedEnv: " << gRTX->getScheduler()->_blockedEnv->toString() << "\n";
//	cout << "Blocking a process : "<< "\n";
//	gRTX->getScheduler()->block_process( 1 );
	
	cout << "_blockedEnv: " << gRTX->getScheduler()->_blockedEnv->toString() << "\n";
	cout << "_readyProcs: " << gRTX->getScheduler()->_readyProcs->toString() << "\n";
	
	cout << "My prioirty is: " << gRTX->_pcbList[7]->getPriority() << "\n";
	cout << "\nTestCCI asks you for input: \n-->";
	cin.get();
	cout << "\nTest CCI acts on your input and releases CPU...\n";
	
	
	gRTX->atomic( 0 );
	gRTX->K_release_processor();
	
	
	
}
}

int createInitTable(PcbInfo* initTable[])
{	
	int ret = EXIT_SUCCESS;

	try	//Assure init table is allocated successfully
	{
//		foo **fooPointer;
//fooPointer = new foo *[10] // memory for an array of 10 pointers

	
		//Loop through each init table entry and allocate memory
		for(int i = 0; i <= PROCESS_COUNT; i++){
			//Do not throw error upon failure, use own validation
			initTable[i] = new PcbInfo();

			//Initialize fields which are consistent across processes
			initTable[i]->processId = i;
			initTable[i]->stackSize = STACK_SIZE;
		}

		initTable[0]->name =		"i_timing";	
		initTable[0]->priority =    0;
		initTable[0]->processType = PROCESS_I;
		initTable[0]->address = 	&(a);

		initTable[1]->name =		"i_kb";	
		initTable[1]->priority =    0;
		initTable[1]->processType = PROCESS_I;
		initTable[1]->address = 	&(b);

		initTable[2]->name =		"i_crt";	
		initTable[2]->priority =    0;
		initTable[2]->processType = PROCESS_I;
		initTable[2]->address = 	&(c);

		initTable[3]->name =		"null_proc";	
		initTable[3]->priority =    3;
		initTable[3]->processType = PROCESS_N;
		initTable[3]->address = 	&(d);

		initTable[4]->name =		"userA";	
		initTable[4]->priority =    2;
		initTable[4]->processType = PROCESS_U;
		initTable[4]->address = 	&(e);

		initTable[5]->name =		"userB";	
		initTable[5]->priority =    2;
		initTable[5]->processType = PROCESS_U;
		initTable[5]->address = 	&(f);

		initTable[6]->name =		"userC";	
		initTable[6]->priority =    2;
		initTable[6]->processType = PROCESS_U;
		initTable[6]->address = 	&(g);
		
		initTable[7]->name =		"testCCI";	
		initTable[7]->priority =    2;
		initTable[7]->processType = PROCESS_K;
		initTable[7]->address = 	&(test_CCI);

	}
	catch(int e)
	{
		delete[] initTable;
		ret = EXIT_ERROR;
	}
	
	return ret;
}
