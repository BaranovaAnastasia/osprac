#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
 
 
int main() {
 
	int fd[2];
 
	if (pipe(fd) < 0) {
		printf("Error");
		exit(-1);
	}

	int pipe_size = (long)fcntl(fd[1], 1032);
 
	printf("Pipe size = %d\n", pipe_size);
	return 0;
}	
