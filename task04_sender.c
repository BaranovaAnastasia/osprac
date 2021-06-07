#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int ready = 0;
int mask = 1;
int integer;

void handler(int nsig) {
	ready = 1;
}

int main(void) {
    pid_t receiverPid;

    printf("pid = %d\n", getpid());

    printf("Receiver pid: ");
    if (scanf("%d", &receiverPid) < 0) {
        printf("Cannot read receiver pid.\n");
        exit(-1);
    }

    printf("Integer to send: ");
    if (scanf("%d", &integer) < 0) {
        printf("Cannot read a number.\n");
        exit(-1);
    }

    signal(SIGUSR1, handler);
    while (mask != 0) {
        while (ready == 0);

		if (integer & mask) {
            kill(receiverPid, SIGUSR2);
        } else {
            kill(receiverPid, SIGUSR1);
        }
        mask <<= 1;
	ready = 0;
    }
    kill(receiverPid, SIGCHLD);

    return 0;
}
