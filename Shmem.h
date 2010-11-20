//#include  <sys/types.h>
//#include <sys/ipc.h>
//#include <sys/shm.h>
//#include <errno.h>
//#include <string.h>
//#include <stdlib.h>

#ifndef H_Shmem
#define H_Shmem

#define BUFSIZE 128
#define MAXDATA 120

//structure used for shared memory

typedef struct 
{
	int busyFlag; //1 if busy, 0 if non-busy
	char data[MAXDATA];
} inputBuffer;

#endif
