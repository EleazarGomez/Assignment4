// Eleazar Gomez, Anton Shirokov
// main.cpp
//
// This file contains the main logic of our program. The main function allocates
// the shared memory segments (4 in total), intitializes them with characters,
// gets input on number of operations for each process to carry out, and forks
// 5 processes to perform swaps on the shared memory. The parent process waits
// for the processes to complete and deallocates the memory. The main function
// uses two other functions to carry out ints function, one for getting user
// input, and one for each child process to execute. The output of the program
// consists of the groups of memory before any processes start, whenever a process
// starts or finishes a critical section, the number of operations a process
// completed, and the groups of memory after all processes finish. To make sure
// that only one process can be executing a critical section at a time, we included
// the semaphore code provided by the professor. We only use one semaphore variable
// (CRITICAL) that starts at 1. We call P before the critical section and V after;
// this setup results in the first process to reach this section being able to
// execute it and each subsequent process being unable to start the section until
// the other(s) are finished.

#include <string>      // string
#include <iostream>    // IO
#include <unistd.h>    // fork
#include <sys/wait.h>  // wait 
#include <sys/shm.h>   // shared mem
#include <limits>      // numeric_limits
#include "constants.h" // our constants
#include "semaphore.h" // code given

using namespace std;

enum {CRITICAL}; // sem var

// Forward declarations
void performProcess(int, SEMAPHORE&, char*, char*, char*, char*);
int getUserInput();


// "main" method. This method, as described above, allocates the shared memory
// segments, initializes them with characters, forks child processes to perform
// atomic swaps, waits, and dealloactes the memory.
//
// Pre-conditions:  None.
// Post-conditions: Processes ran: memory was allocated, altered, then destroyed.
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
	for (int i = 1; i < NUM_PROCESSES + 1; i++)
	{
		long childPID = 0; // Stores PID of child (0 if it is the child)

		childPID = fork();

		// Set unique random seed for each process
		// Source:
		// https://stackoverflow.com/questions/12779235/
		//         how-to-properly-choose-rng-seed-for-parallel-processes
		srand(time(NULL) * i * getpid());

		if (childPID == 0) // CHILD
		{
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

// "getUserInput" method. This method takes no parameters. Its only function is
// to retrieve the number of operations for each process to execute and return it.
// This value can not be 0 or less. The function checks for invalid input.
//
// Pre-conditions:  None.
// Post-conditions: Integer representing the number of operations is returned.
int getUserInput()
{
	int numberOfOperations = 0;

	while (true)
	{
		cout << "Enter # of process operations: ";
		cin >> numberOfOperations;
		
		if (cin.fail())
		{
			// Reset variable, clear input, and output error message.
			numberOfOperations = 0;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input." << endl;
		}
		else if (numberOfOperations < 1)
		{
			// Reset variable, output error message (invalid input).
			numberOfOperations = 0;
			cout << "Invalid input." << endl;
		}
		else
		{
			// Valid input.
			break;
		}
	}
	
	return numberOfOperations;
}


// "performProcess" method. This is the method that each process will be
// executing until it completes its required number of operations. The method
// takes 6 parameters: the number of operations to complete (int), the sem var
// (SEMAPHORE&), and the four shared memory segments (char*). The operations
// take place in an infinite while loop that is only broken when the process
// completes its operations. The first operation is to generate a random
// 32 bit integer, and check if its less than the number provided in the lab
// instructions in order to slow the process down. If this occurs the main
// functionality of the function takes place: the random choosing of two of
// the segments and one chunk in each section, and the atomic swapping of
// these chunks. The sem var is incremented and decremented in order to prevent
// two processes from performing this section at once.
//
// Pre-conditions:  The four shared memory segments have been allocated,
//                  the number of operations has been retrieved, and the
//                  sem var has been intialized.
// Post-conditions: The process has completed and is returning the main function.
void performProcess(int numberOfOperations, SEMAPHORE& sem,
			  char* str1, char* str2, char* str3, char* str4)
{
	int speed_check = 0;        // will hold random variable to slow down
	int operationsComplete = 0; // counts number of operations we have completed

	while (true)
	{
		speed_check = rand(); // Generate random 32 bit integer.

		// If less than 5000;
		if (speed_check < SPEED)
		{
			// Get two random groups (1 - 4) that are not the same.
			int group1;
			int group2;
		
			group1 = (rand() % NUM_GROUPS) + 1;
			
			group2 = group1;

			while (group2 == group1)
			{
				group2 = (rand() % NUM_GROUPS) + 1;
			}

			// Get ranom chunks for the groups.
			int group1chunk = (rand() % NUM_CHUNKS) + 1;
			int group2chunk = (rand() % NUM_CHUNKS) + 1;

			// Based on the random groups, choose the correct shared segment.
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

			int offset = 0;    // used to index second chunk
			bool first = true; // flag checking for first iteration

			// Swap
			sem.P(CRITICAL); // Decrement sem
			for (int i = g1_start; i < g1_start + CHUNK_SIZE; i++)
			{
				if (first)
				{
					// Starting critical section
					first = false;
					cout << "Process " << getpid() << " swapping" << endl;
				}
				// Swap algorithm
				char temp = *(mem1 + i);
				*(mem1 + i) = *(mem2 + g2_start + offset);
				*(mem2 + g2_start + offset) = temp;
				offset++;
			}
			// Critical secion over
			cout << "Process " << getpid() << " done swapping" << endl;
			sem.V(CRITICAL); // Increment sem (P calls == V calls)

			operationsComplete++;

			// If we match the user's input, break the while loop
			if (operationsComplete == numberOfOperations)
			{
				// This output shows there was no deadlock.
				cout << endl;
				cout << getpid() << " completed " << operationsComplete;
				cout << " operations" << endl;
				cout << endl;
				break;
			}
		}
	}
}
