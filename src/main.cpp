//#include <string>
#include <iostream>
#include <unistd.h>    // fork
#include <sys/wait.h>  // wait 
#include <sys/shm.h>   // shared mem

using namespace std;

int main() 
{
	int segmentID;

	const int segmentSize = 1024;

	key_t key = ftok("shmfile", 65); 

	// Allocate a shared memory segment.
	segmentID = shmget(key, segmentSize, 0666 | IPC_CREAT);

	// Attach the shared memory segment.
	char* str = (char*) shmat(segmentID, (void*) 0, 0);

	// Write a string to the shared memory segment.
	sprintf(str, "Hello, world.");
	printf("Parent prints: %s\n", str);
	
	// Fork
	long childPID = 0;

	childPID = fork();

	if (childPID == 0) // CHILD
	{
		sprintf(str, "Goodbye, world.");
		exit(0);
	}
	else // PARENT
	{
		// Print out the string from shared memory.
		wait(NULL);
		printf("Parent reads: %s\n", str);
		// Detach
		shmdt(str);
		shmctl(segmentID, IPC_RMID, 0);
	}

	return 0; 
} 
