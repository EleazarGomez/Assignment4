#include <string>      // string
#include <iostream>    // IO
#include <unistd.h>    // fork
#include <sys/wait.h>  // wait 
#include <sys/shm.h>   // shared mem
#include <limits>      // numeric_limits
#include "constants.h" // our constants
#include "semaphore.h" // code given

using namespace std;

enum {CRITICAL};

void performProcess(int, SEMAPHORE&, char*, char*, char*, char*);
int getUserInput();

int main()
{
	// Set seed
	srand(time(NULL));

	// Allocate a shared memory segments.
	int segmentID1 = shmget(IPC_PRIVATE, GROUP_SIZE * sizeof(char), PERMS);
	int segmentID2 = shmget(IPC_PRIVATE, GROUP_SIZE * sizeof(char), PERMS);
	int segmentID3 = shmget(IPC_PRIVATE, GROUP_SIZE * sizeof(char), PERMS);
	int segmentID4 = shmget(IPC_PRIVATE, GROUP_SIZE * sizeof(char), PERMS);

	// Attach to segments
	char* str1 = (char*) shmat(segmentID1, 0, SHM_RND);
	char* str2 = (char*) shmat(segmentID2, 0, SHM_RND);
	char* str3 = (char*) shmat(segmentID3, 0, SHM_RND);
	char* str4 = (char*) shmat(segmentID4, 0, SHM_RND);

	// Intialize chunk data
	for (int i = CHUNK1_START; i <= CHUNK3_END; i++)
	{
		*(str1 + i) = LOWER_CASE_LETTERS[rand() % L_SIZE];
	}

	for (int i = CHUNK1_START; i <= CHUNK3_END; i++)
	{
		*(str2 + i) = UPPER_CASE_LETTERS[rand() % L_SIZE];
	}

	for (int i = CHUNK1_START; i <= CHUNK3_END; i++)
	{
		*(str3 + i) = UPPER_CASE_LETTERS[rand() % L_SIZE];
	}

	for (int i = CHUNK1_START; i <= CHUNK3_END; i++)
	{
		*(str4 + i) = UPPER_CASE_LETTERS[rand() % L_SIZE];
	}

	// Get input on number of operations
	int numberOfOperations = getUserInput();

	// Print intial data
	printf("\n\nGroup1:\n\n%s\n", str1);
	printf("\n\nGroup2:\n\n%s\n", str2);
	printf("\n\nGroup3:\n\n%s\n", str3);
	printf("\n\nGroup4:\n\n%s\n\n\n", str4);

	// Initialize semaphore
	SEMAPHORE sem(1);
	sem.V(CRITICAL);

	// Fork
	for (int i = 1; i < NUM_PROCESSES + 1; i++) // NUM_PROCESSES
	{
		long childPID = 0;

		childPID = fork();

		// Set random seed for each process
		srand(time(NULL) * i * getpid());

		if (childPID == 0) // CHILD
		{
			//cout << "Process " << i << endl;

			performProcess(numberOfOperations, sem, str1, str2, str3, str4);

			// Exit child process once finished
			exit(0);
		}
	}

	// Wait for all processes to finish
	wait(NULL);
	wait(NULL);
	wait(NULL);
	wait(NULL);
	wait(NULL);

	// Print results
	printf("\n\nGroup1:\n\n%s\n", str1);
	printf("\n\nGroup2:\n\n%s\n", str2);
	printf("\n\nGroup3:\n\n%s\n", str3);
	printf("\n\nGroup4:\n\n%s\n", str4);
	
	// Detach from segments
	shmdt(str1);
	shmdt(str2);
	shmdt(str3);
	shmdt(str4);

	// Destroy segment memory
	shmctl(segmentID1, IPC_RMID, NULL);
	shmctl(segmentID2, IPC_RMID, NULL);
	shmctl(segmentID3, IPC_RMID, NULL);
	shmctl(segmentID4, IPC_RMID, NULL);

	return 0; 
}

int getUserInput()
{
	int numberOfOperations = 0;

	while (true)
	{
		cout << "Enter # of process operations: ";
		cin >> numberOfOperations;
		
		if (cin.fail())
		{
			numberOfOperations = 0;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input." << endl;
		}
		else if (numberOfOperations < 1)
		{
			numberOfOperations = 0;
			cout << "Invalid input." << endl;
		}
		else
		{
			break;
		}
	}
	
	return numberOfOperations;
}

void performProcess(int numberOfOperations, SEMAPHORE& sem,
			  char* str1, char* str2, char* str3, char* str4)
{
	int speed_check = 0;
	int operationsComplete = 0;

	while (true)
	{
		speed_check = rand();

		if (speed_check < SPEED)
		{
			int group1;
			int group2;
		
			group1 = (rand() % NUM_GROUPS) + 1;
			
			group2 = group1;

			while (group2 == group1)
			{
				group2 = (rand() % NUM_GROUPS) + 1;
			}

			int group1chunk = (rand() % NUM_CHUNKS) + 1;
			int group2chunk = (rand() % NUM_CHUNKS) + 1;

			char* mem1;
			char* mem2;

			switch (group1)
			{
				case 1:
					mem1 = str1;
					break;
				case 2:
					mem1 = str2;
					break;
				case 3:
					mem1 = str3;
					break;
				case 4:
					mem1 = str4;
					break;
			}


			switch (group2)
			{
				case 1:
					mem2 = str1;
					break;
				case 2:
					mem2 = str2;
					break;
				case 3:
					mem2 = str3;
					break;
				case 4:
					mem2 = str4;
					break;
			}

			// Get starting indexes
			int g1_start = 0;
			int g2_start = 0;

			// Offset based on chunk
			g1_start += (group1chunk - 1) * CHUNK_SIZE;
			g2_start += (group2chunk - 1) * CHUNK_SIZE;

			int offset = 0;
			bool first = true;

			// Swap
			//cout << "Process " << getpid() << " attempting to swap" << endl;
			sem.P(CRITICAL);
			for (int i = g1_start; i < g1_start + CHUNK_SIZE; i++)
			{
				if (first)
				{
					first = false;
					cout << "Process " << getpid() << " swapping" << endl;
				}
				char temp = *(mem1 + i);
				*(mem1 + i) = *(mem2 + g2_start + offset);
				*(mem2 + g2_start + offset) = temp;
				offset++;
			}
			cout << "Process " << getpid() << " done swapping" << endl;
			sem.V(CRITICAL);

			operationsComplete++;

			if (operationsComplete == numberOfOperations)
			{
				break;
			}
		}
	}
}
