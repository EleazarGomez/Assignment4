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

	struct shmid_ds shmbuffer; 

	int segment_size; 

	const int shared_segment_size = 1024;

	key_t key = ftok("shmfile",65); 

	/* Allocate a shared memory segment.  */
	segment_id = shmget(key, shared_segment_size,
				  0666 | IPC_CREAT);

	/* Attach the shared memory segment.  */
	char* str = (char*) shmat(segment_id, (void*) 0, 0);

	printf("shared memory attached at address %p\n", str);

	/* Determine the segment's size. */
	shmctl(segment_id, IPC_STAT, &shmbuffer);

	segment_size = shmbuffer.shm_segsz;

	printf("segment size: %d\n", segment_size);

	/* Write a string to the shared memory segment.  */
	sprintf(str, "Hello, world.");

	/* Detach the shared memory segment.  */ 
	shmdt(str);

	return 0; 
} 
