#include "rtx.h"
#include "data_structures/PcbInfo.h"
#include "signal.h"

//Private method declarations
int inititalizeShmem();
int cleanupShmem();
int createInitTable(PcbInfo* initTable[]);
int cleanupInitTable();

int main()
{
	PcbInfo* initTable[PROCESS_COUNT];
	int pidKB, pidCRT, pidMe;
	pidMe = getpid();

	debugMsg("\n------------------------------------\n           RTX INITIALIZED\n------------------------------------\n\n");	
	
	assure(inititalizeShmem() == EXIT_SUCCESS, "Shared memory failed to initialize", __FILE__, __LINE__, true);
	assure(createInitTable(initTable) == EXIT_SUCCESS, "Init table failed to initialize", __FILE__, __LINE__, true);

	if ((pidKB = fork()) == 0)
	{
		debugMsg("Keyboard forked\n");
		sleep(1000000000);
		assure(false, "Keyboard helper process failed to initialize", __FILE__, __LINE__, true);
		exit(1);
	}
	if ((pidCRT = fork()) == 0)
	{
		debugMsg("CRT forked\n");
		sleep(1000000000);
		assure(false, "CRT helper process failed to initialize", __FILE__, __LINE__, true);
		exit(1);
	}
	sleep(1);

	
	//Rtx rtx = new RTX(initTable);

	assure(cleanupInitTable() == EXIT_SUCCESS, "Init table cleanup failed", __FILE__, __LINE__, false);
	assure(cleanupShmem() == EXIT_SUCCESS, "Shared memory cleanup failed", __FILE__, __LINE__, false);
	
	
	kill(pidKB,SIGKILL);
	wait();	
	kill(pidCRT,SIGKILL);
	wait();	

	debugMsg("\n------------------------------------\n    RTX TERMINATED SUCCESSFULLY\n------------------------------------\n\n");	
	return EXIT_SUCCESS;
}

int inititalizeShmem()
{
/*
	for(2 iterations)    //set up the file

	{

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
	return EXIT_SUCCESS;
}

int createInitTable(PcbInfo* initTable[])
{	
	for(int i = 0; i <= PROCESS_COUNT; i++){
		initTable[i] = new(std::nothrow) PcbInfo();
		if (initTable[i] == NULL)	
			return EXIT_ERROR;
		
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

int cleanupInitTable()
{
/*
	
	kill(keyboard_process, TERMINATE)
	wait();
	kill(crt_process, TERMINATE)
	wait();

	munmap (rx_memmap_pt)        //unmap the memory buffers
	munmap (tx_memmap_pt)

	close(rx_shared_mem_f_id)    //close temporary files
	close(tx_shared_mem_f_id)

	unlink(rx_shared_mem_f)    //delete temporary files
	unlink(tx_shared_mem_f)
*/
	return EXIT_SUCCESS;
}
