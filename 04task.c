#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
	pid_t pid;

	pid = fork();

	if (pid == -1) {
		exit(1);
	} else if (pid == 0) {
		printf("I'm a child. Look at this:\n");
		(void) execle("/bin/cat", "/bin/cat", "03task.c", 0, envp);
	} else {
		printf("I'm a parent\n");
	}

	return 0;
}
