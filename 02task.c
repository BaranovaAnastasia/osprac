#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	pid_t pid;

	pid = fork();

	if (pid == -1) {
		printf("I'm an error\n");
	} else if (pid == 0) {
		printf("I'm a child\n");
	} else {
		printf("I'm a parent\n");
	}

	return 0;
}
