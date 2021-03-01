#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main() {
	char* text;
	int size;
	int* ptr;
 
	int     shmid;
	char    pathname[] = "01task_writer.c";
	key_t   key;
	int     new = 1;

	FILE* file = fopen(pathname, "r");
	fseek(file, 0L, SEEK_END);
	size = ftell(file);
	rewind(file);
 
	if ((key = ftok(pathname, 0)) < 0) {
		printf("Can\'t generate key\n");
		exit(-1);
	}

	if ((shmid = shmget(key, sizeof(int) + size * sizeof(char), 0666|IPC_CREAT)) < 0) {
		printf("Can\'t create shared memory\n");
        	exit(-1);
	}

	if ((ptr = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
		printf("Can't attach shared memory\n");
		exit(-1);
	}

	*ptr = size;
	text = (char*)(ptr + 1);

	for (int i = 0; i < size; ++i) {
		text[i] = fgetc(file);
	}

	fclose(file);
	
	if (shmdt(ptr) < 0) {
		printf("Can't detach shared memory\n");
		exit(-1);
	}

	return 0;
}
