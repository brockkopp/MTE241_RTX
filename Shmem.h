//#include <sys/ipc.h>
//#include <sys/shm.h>
//#include <fcntl.h>

#include  <sys/types.h> //off_t
#include <string.h>
#include <stdlib.h> //exit
#include <stdio.h> //printf, getchar
#include <signal.h> //sigset
#include <unistd.h> //usleep
#include <sys/mman.h> //mmap, flags
#include <sys/wait.h> //kill, sigset

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
