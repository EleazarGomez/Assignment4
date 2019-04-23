#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semaphore.h"

const int MAXCHAR = 10;
const int BUFFSIZE = 3;
enum {PUT_ITEM, TAKE_ITEM}; // set up names of my 2 semaphores
//       0          1

void producer_proc(SEMAPHORE &, char *);
void parent_cleanup(SEMAPHORE &, int);
void consumer_proc(SEMAPHORE &, char *);

int main(){
	// shared mem
	int shmid;
	char *shmBUF;

	SEMAPHORE sem(2); // start with zero init vals, array of 2 semaphores (0, 1 --> enum)
	sem.V(PUT_ITEM); // + 1 .... could have sed V(0), less readable, index for array not semaphore values
	sem.V(PUT_ITEM); // + 1
	sem.V(PUT_ITEM); // + 1
	                 // -----
					 //   3

	//shmem(shared mem)
	shmid = shmget(IPC_PRIVATE, BUFFSIZE*sizeof(char), PERMS); // similar to msgget, allocate shared memory, IPC_PRIVATE replaces ftok
	shmBUF = (char *)shmat(shmid, 0, SHM_RND); // attach to shmem
	                    //"memory management"
		
	if(fork()){ /* parent process */

		producer_proc(sem, shmBUF);
		parent_cleanup(sem, shmid);

	} else { // child process
		consumer_proc(sem, shmBUF);
	}

	exit(0);
} // main

void consumer_proc(SEMAPHORE &sem, char *shmBUF) {
	char tmp;

	for(int k=0; k<MAXCHAR; k++){
		sem.P(TAKE_ITEM);
		tmp = *(shmBUF+k%BUFFSIZE); // shmBUF is a pointer to the semids (PIC)
		sem.V(PUT_ITEM);
		cout << "(" << getpid() << ")  "  // getpid useful for debugging (tag output)
				<< "buf[" << k%BUFFSIZE << "] "
				<< tmp << endl;
	}
} // child_proc

void producer_proc(SEMAPHORE &sem, char *shmBUF) {
	// producing
	char data[128];
	cout << "(" << getpid() << ")  Please enter a string --> ";
	cin.getline(data, 127);

	char input;
	for(int k=0; k<MAXCHAR; k++){
		input = data[k]; // produce
		sem.P(PUT_ITEM); // P(PUT_ITEM)
		*(shmBUF+(k%BUFFSIZE)) = input; // copy item
		sem.V(TAKE_ITEM);
	}
} // parent_proc

void parent_cleanup (SEMAPHORE &sem, int shmid) {

	int status;			/* child status */ // never used
	wait(0);	/* wait for child to exit */
	shmctl(shmid, IPC_RMID, NULL);	/* cleaning up */
	sem.remove();
} // parent_cleanup

