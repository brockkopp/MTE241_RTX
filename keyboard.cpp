#include <stdio.h> //printf(), getchar()
#include <stdlib.h> //for exit()
#include <signal.h> //sigset
//#include <fcntl.h>
#include <sys/mman.h> //mmap, flags
#include <sys/types.h> //off_t
#include <sys/wait.h> //kill, sigset

#include "Shmem.h"

#include <iostream>
//#include "debug.h"

//using namespace std;

void keyboard_die(int signal)
{
	exit(0); //don't have to do anything since the RTX cleans up the shared memory
}

int main(int arg1, char* arg[])
{
	std::cout<<"KB  child initialized\n";
	//debugMsg("KB  child initialized\n");
	
	sigset(SIGINT,keyboard_die); //set signal handler in case parent process terminates us
	
	int parentPid, fileId;
	caddr_t rx_mmap_ptr;
	
	// get ids of parent process and file (passed as input arguments) to be used for signal sending and mapping purposes
	sscanf(arg[1], "%d", &parentPid );
	sscanf(arg[2], "%d", &fileId ); 

	//now perform actual mapping of the shared memory file via the inputbuffer *
	
	rx_mmap_ptr = (char*)mmap((caddr_t) 0,  //memory Location, 0 lets OS choose
														BUFSIZE,								//the number of bytes to map
														PROT_READ | PROT_WRITE, //read/write permissions
														MAP_SHARED,    					//indicate it's accesible by another process
														fileId,           			//fileId of the file associated with the memory mapping
														(off_t) 0);    					//offset in page frame
    if (rx_mmap_ptr == MAP_FAILED) //ensure mapping was successful; if not, this is a fatal error
    {
      printf("Memory Mapping in Keyboard Child Has Failed. Keyboard is ABORTING!\n");
	  	keyboard_die(0);
    }
	
	inputBuffer* rx_mem_buf = (inputBuffer*) rx_mmap_ptr;  //rx_mem_buf is now a pointer to mapped shared memory! :)

	//polling to read input from the keyboard
	int indexInBuf = 0;
	rx_mem_buf->busyFlag = 0; 
	char userInput;
	do
	{
		userInput = getchar();
		if (userInput == '\n') //indicates end of message. Keyboard process must add information to the shared memory and send a signal to the RTX
		{
			rx_mem_buf->data[indexInBuf] = '\n';
			rx_mem_buf->busyFlag = 1; //set flag that keyboard is "busy" i.e. trying to transmit something from shared memory to parent process
			kill(parentPid, SIGUSR1); //send a signal to the RTX indicating that data has been provided by the user -> COMPLETE MESSAGE SENT
			//do not reset indexInBuf until the i_keyboard_handler resets the busyFlag after extracting all information from shmem
			while(rx_mem_buf->busyFlag == 1) //wait for i_keyboard_handler to process signal
				usleep(100000); //wait 10^5 usec, or 0.1sec   !!!This may be considered an error on some systems; must be min 1000000 sometimes!
			
			//at this point, after leaving the while loop, this means the i_keyboard_handler has extracted information from the shared memory and reset the busy flag
			//this means the keyboard process is ready to take in more information! Reset the indexInBuf:
			indexInBuf = 0; 
		}	
		else //user is still inputting data...
		{
			rx_mem_buf->data[indexInBuf] = userInput;
			indexInBuf++;
			//check if memory is full!
			if(indexInBuf == MAXDATA)
			{
				rx_mem_buf->busyFlag = 1; //set flag that keyboard is "busy" i.e. trying to transmit something from shared memory to parent process
				kill(parentPid, SIGUSR1); //send a signal to the RTX indicating that data has been provided by the user -> MEMORY IS FULL!
				while(rx_mem_buf->busyFlag == 1) //wait for i_keyboard_handler to process signal and resets the busyFlag after extracting all information from shmem
					usleep(100000); //wait 10^5 usec, or 0.1sec   !!!This may be considered an error on some systems; must be min 1000000 sometimes!
			}			
		}
	}	
	while(1);  //an infinite loop - exit when parent signals us

	return 0;
}
