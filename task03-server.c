#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


int main() {
    int msqid;
    char pathname[] = "task03_server.c";
    key_t key;
    int len, max;

    struct clientmsgbuf {
        long mtype;
        struct {
            pid_t pid;
            float message;
        } info;
    } clientbuf;

    struct servermsgbuf {
        long mtype;
        struct {
            float message;
        } info;
    } serverbuf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can't generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can't get msqid\n");
        exit(-1);
    }

    while (true) {
        max = sizeof(clientbuf.info);
        if (len = msgrcv(msqid, (struct clientmsgbuf *) &clientbuf, max, 1, 0) < 0) {
            printf("Can't receive message from queue\n");
            exit(-1);
        }
        printf("From client with pid = %d message '%f' was received.\n", clientbuf.info.pid, clientbuf.info.message);

        serverbuf.mtype = clientbuf.info.pid;
        float number = clientbuf.info.message;
        serverbuf.info.message = number * number;
        len = sizeof(serverbuf.info);

        if (msgsnd(msqid, (struct servermsgbuf *) &serverbuf, len, 0) < 0) {
            printf("Can't send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
        printf("Result is returned to the client with pid = %d\n", clientbuf.info.pid);
    }
    return 0;
}