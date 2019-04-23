#include <string>
#include <iostream>
#include <unistd.h>    // fork
#include <sys/wait.h>  // wait 
#include <sys/shm.h>   // shared mem

using namespace std;

int main() 
{
	int segmentID;

	const int segmentSize = 6144; // Enough for the 4 groups of 3 chunks

	// Allocate a shared memory segment.
	segmentID = shmget(IPC_PRIVATE, segmentSize, 0666 | IPC_CREAT);

	// Attach to the shared memory segment.
	char* str = (char*) shmat(segmentID, 0, SHM_RND);

	// Write a string to the shared memory segment.
	sprintf(str, "Hello, world.");
	printf("Parent prints: %s\n", str);
	
	// Fork
	for (int i = 1; i < 6; i++)
	{
		long childPID = 0;

		childPID = fork();

		if (childPID == 0) // CHILD
		{
			cout << "Process " << i << endl;
			sprintf(str, "Goodbye, world.");
			exit(0);
		}
	}

	// Wait for all processes to finish
	wait(NULL);
	wait(NULL);
	wait(NULL);
	wait(NULL);
	wait(NULL);

	// Print results, detach, and destroy
	printf("Parent reads: %s\n", str);
	shmdt(str); // detach
	shmctl(segmentID, IPC_RMID, NULL); // destroy memory

	return 0; 
} 
