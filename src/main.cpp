#include <string>
#include <iostream>
#include <unistd.h>    // fork
#include <sys/wait.h>  // wait 
#include <sys/shm.h>   // shared mem
#include "constants.h"

using namespace std;

int main() 
{
	int segmentID;

	// Allocate a shared memory segment.
	segmentID = shmget(IPC_PRIVATE, SEGMENT_SIZE, 0666 | IPC_CREAT);

	// Attach to the shared memory segment.
	char* str = (char*) shmat(segmentID, 0, SHM_RND);

	// Intialize data
	for (int i = G1C1_START; i <= G1C3_END; i++)
	{
		*(str + i) = LOWER_CASE_LETTERS[rand() % L_SIZE];
	}

	for (int i = G2C1_START; i <= G4C3_END; i++)
	{
		*(str + i) = UPPER_CASE_LETTERS[rand() % L_SIZE];
	}

	/*
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
			char data[10];
			for (int i = 0; i < 10; i++)
			{
				data[i] = 'a';
			}

			cout << "Process " << i << endl;

			for (int i = 0; i < 10; i++)
			{
				char input = data[i];
				*(str + i) = input;
			}

			exit(0);
		}
	}

	// Wait for all processes to finish
	wait(NULL);
	wait(NULL);
	wait(NULL);
	wait(NULL);
	wait(NULL);
	*/

	// Print results, detach, and destroy
	printf("Parent reads: %s\n", str);
	shmdt(str); // detach
	shmctl(segmentID, IPC_RMID, NULL); // destroy memory

	return 0; 
} 
