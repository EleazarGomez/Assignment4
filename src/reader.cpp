#include <stdio.h> 
#include <sys/shm.h> 
#include <sys/stat.h> 
#include <string>
#include <vector>
#include <iostream>

using namespace std;


int main()
{
	int segment_id;

	const int shared_segment_size = 1024;

	key_t key = ftok("shmfile", 65); 

	/* Allocate a shared memory segment.  */
	segment_id = shmget(key, shared_segment_size,
				  0666 | IPC_CREAT);

	/* Reattach the shared memory segment, at a different address.  */
	char* str = (char*) shmat(segment_id, (void*) 0, 0);

	printf("shared memory reattached at address %p\n", str);

	/* Print out the string from shared memory.  */
	printf("%s\n", str); 

	/* Detach the shared memory segment.  */
	shmdt(str);

	/* Deallocate the shared memory segment.  */
	shmctl(segment_id, IPC_RMID, 0);
}
