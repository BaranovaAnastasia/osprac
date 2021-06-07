#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) {
    pid_t pid, receiverPid;
    int integer;
    int mask = 1;

    pid = getpid();
    printf("pid = %d\n", pid);

    printf("Receiver pid: ");
    scanf("%d", &receiverPid);

    printf("Integer to send: ");
    scanf("%d", &integer);

    while (mask != 0) {
        if (integer & mask) {
            kill(receiverPid, SIGUSR2);
        } else {
            kill(receiverPid, SIGUSR1);
        }
        mask <<= 1;
    }

    return 0;
}