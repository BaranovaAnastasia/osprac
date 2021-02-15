#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
	int     fd;
	size_t  size;
	char    string[] = "Hello, world!";
  
	// Write
	(void)umask(0);
  
	if ((fd = open("myfile", O_WRONLY | O_CREAT, 0666)) < 0) {
		printf("Cannot open file\n");
		exit(-1);
	}
  
	size = write(fd, string, 14);

	if (size != 14) {
		printf("Cannot write all string\n");
		exit(-1);
	}

	if (close(fd) < 0) {
		printf("Cannot close file\n");
		exit(-1);
	}


	
	// Read
	if ((fd = open("myfile", O_RDONLY, 0666)) < 0) {
		printf("Cannot open file\n");
		exit(-1);
	}

	char  res[14];
	size = read(fd, res, 14);

	if (size < 0) {
		printf("Cannot read file\n");
		exit(-1);
	}

	printf("%s\n", res);


	if (close(fd) < 0) {
		printf("Cannot close file\n");
	}


	return 0;
}
