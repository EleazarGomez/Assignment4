#include <string>
#include <iostream>
#include <unistd.h>    // fork
#include <sys/wait.h>  // wait 
#include <sys/shm.h>   // shared mem
#include <limits>      // numeric_limits
//#include <sys/types.h> // getpid?
//#include <time.h>      // time
#include "constants.h"

using namespace std;

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
		else
		{
			break;
		}
	}
	

	// Fork
	for (int i = 1; i < NUM_PROCESSES + 1; i++)
	{
		long childPID = 0;

		childPID = fork();

		// Set random seed for each process
		srand(time(NULL) * i * getpid());

		if (childPID == 0) // CHILD
		{
			cout << "Process " << i << endl;

			int speed_check = 0;

			while (true)
			{
				speed_check = rand();

				if (speed_check < 5000)
				{
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
					break;
				}
			}

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
	//printf("Data:\n\n%s\n", str);
	shmdt(str); // detach
	shmctl(segmentID, IPC_RMID, NULL); // destroy memory

	return 0; 
} 
