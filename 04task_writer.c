#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	int fd;
	size_t size;
	char name[] = "fifo2.fifo";

	(void)umask(0);
 
	// Fifo creating
	if (mknod(name, S_IFIFO | 0666, 0) < 0) {
		printf("Can\'t create FIFO\n");
		exit(-1);
	}

	// Writing.
	printf("To continue start program from '04task_reader.c'\n");
	if ((fd = open(name, O_WRONLY)) < 0) {
		printf("Can\'t open FIFO for writting\n");
		exit(-1);
	}
 
	size = write(fd, "Hello, world!", 14);

	if (size != 14) {
		printf("Can\'t write all string to FIFO\n");
		exit(-1);
	}

	if (close(fd) < 0) {
		printf("writer: Can\'t close FIFO\n");
		exit(-1);
	}

	printf("Writer exit\n");
	return 0;
}
