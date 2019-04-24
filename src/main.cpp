#include <string>      // string
#include <iostream>    // IO
#include <unistd.h>    // fork
#include <sys/wait.h>  // wait 
#include <sys/shm.h>   // shared mem
#include <limits>      // numeric_limits
#include "constants.h" // our constants
#include "semaphore.h" // code given

using namespace std;

void performProcess(int, char*);
int getUserInput();

int main() 
{
	int segmentID;

	srand(time(NULL));

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

	// Get input on number of operations
	int numberOfOperations = getUserInput();

	// Fork
	for (int i = 1; i < 1 + 1; i++) // NUM_PROCESSES
	{
		long childPID = 0;

		childPID = fork();

		// Set random seed for each process
		srand(time(NULL) * i * getpid());

		if (childPID == 0) // CHILD
		{
			cout << "Process " << i << endl;

			performProcess(numberOfOperations, str);

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

	// Print results, detach, and destroy
	printf("\n\nData:\n\n%s\n", str);
	shmdt(str); // detach
	shmctl(segmentID, IPC_RMID, NULL); // destroy memory

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

void performProcess(int numberOfOperations, char* str)
{
	int speed_check = 0;
	int operationsComplete = 0;

	while (true)
	{
		speed_check = rand();

		if (speed_check < 5000)
		{
			cout << operationsComplete << endl;
			cout << "\tspeed check: " << speed_check << endl;

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


			cout << "\t\tGroup " << group1 << " Chunk " <<
				  group1chunk << endl;
			cout << "\t\tGroup " << group2 << " Chunk " <<
				  group2chunk << endl;

			// Get starting indexes
			int g1_start = 0;
			int g2_start = 0;

			// Offset based on group
			g1_start += (group1 - 1) * GROUP_SIZE;
			g2_start += (group2 - 1) * GROUP_SIZE;

			// Offset based on chunk
			g1_start += (group1chunk - 1) * CHUNK_SIZE;
			g2_start += (group2chunk - 1) * CHUNK_SIZE;

			int offset = 0;

			// Swap
			for (int i = g1_start; i < g1_start + CHUNK_SIZE; i++)
			{
				char temp = *(str + i);
				*(str + i) = *(str + g2_start + offset);
				*(str + g2_start + offset) = temp;
				offset++;
			}

			operationsComplete++;

			if (operationsComplete == numberOfOperations)
			{
				break;
			}
		}
	}
}
