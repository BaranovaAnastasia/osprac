#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int value = 0;
int mask = 1;

void setZero(int nsig) {
    mask <<= 1;
}

void setOne(int nsig) {
    value |= mask;
    mask <<= 1;
}

int main(void) {
    pid_t pid, senderPid;

    (void) signal(SIGUSR1, setZero);
    (void) signal(SIGUSR2, setOne);

    pid = getpid();
    printf("pid = %d\n", pid);

    printf("Sender pid: ");
    scanf("%d", &senderPid);

    while (1) {
        if(mask == 0) {
            printf("Received value: %d\n", value);
            break;
        }
    }
    return 0;
}
