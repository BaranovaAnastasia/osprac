#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int value = 0;
int mask = 1;
int ready = 0;
pid_t senderPid;

void handler1(int nsig) {
    mask <<= 1;
    kill(senderPid, SIGUSR1);
}

void handler2(int nsig) {
    value |= mask;
    mask <<= 1;
    kill(senderPid, SIGUSR1);
}

void handler(int nsig) {
    ready = 1;
}

int main(void) {
    printf("pid = %d\n", getpid());

    printf("Sender pid: ");
    if (scanf("%d", &senderPid) < 0) {
        printf("Cannot read sender pid.\n");
        exit(-1);
    }

    kill(senderPid, SIGUSR1);

    (void) signal(SIGUSR1, handler1);
    (void) signal(SIGUSR2, handler2);
    (void) signal(SIGCHLD, handler);

    while(ready == 0);
    printf("Received value: %d\n", value);
    
    return 0;
}
