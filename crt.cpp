#include "Shmem.h"
#include "debug.h"

void die(int signal)
{
	exit(0); //don't have to do anything since the RTX cleans up the shared memory
}

int main(int arg1, char* arg[])
{
	debugMsg("CRT child initialized\n");
	
	sigset(SIGINT, die); //set signal handler in case parent process terminates us
	
	int parentPid, fileId;
	caddr_t tx_mmap_ptr;
	
	// get ids of parent process and file (passed as input arguments) to be used for signal sending and mapping purposes
	sscanf(arg[1], "%d", &parentPid );
	sscanf(arg[2], "%d", &fileId ); 

	//now perform actual mapping of the shared memory file via the inputbuffer*
	
	tx_mmap_ptr = (char*)mmap((caddr_t) 0,  //memory Location, 0 lets OS choose
							BUFSIZE,								//the number of bytes to map
							PROT_READ | PROT_WRITE, //read/write permissions
							MAP_SHARED,    					//indicate it's accesible by another process
							fileId,           			//fileId of the file associated with the memory mapping
							(off_t) 0);    					//offset in page frame
  if (tx_mmap_ptr == MAP_FAILED) //ensure mapping was successful; if not, this is a fatal error
  {
  	printf("Memory Mapping in CRT Child Has Failed. CRT is ABORTING!\n");
	die(0);
  }
	
	inputBuffer* tx_mem_buf = (inputBuffer*) tx_mmap_ptr;  //x_mem_buf is now a pointer to mapped shared memory! :)

	//polling to read input from the keyboard
	if(tx_mem_buf->busyFlag == 1) //synchronized with crt_i_process; set to 1 once iprocess started inputting values
	{
		int indexInBuf = 0;
		char c = tx_mem_buf->data[indexInBuf];
		while (c != '\0')
		{
			cout<<c;
			indexInBuf++;
			c = tx_mem_buf->data[indexInBuf];
			if(indexInBuf == MAXDATA) //should  never happen
				break;
		}
		tx_mem_buf->busyFlag = 0; //indicate that the entire message has been transmitted and the crt process is no longer busy
		cout<<'\n';
	}
	return 0;
}
