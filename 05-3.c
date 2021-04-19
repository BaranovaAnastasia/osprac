// В начале семафор инициализируется 1. Родитель записывает сообщение, 
// затем вызывается A(S, 1) и 
// далее ребенок производит чтение. После этого, родитель через D(S, 1) ждет, 
// когда ребенок запишет сообщение и вызовет A(S, 1). 
// Это повторяется n раз.

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
 
int main()
{
    int     fd[2], result;
    key_t   key;
    int     semid;
    struct  sembuf mybuf;
    char    pathname[] = "05-3.c";
    size_t  size;
    char    resstring[14];
 
    scanf("n = %d", &n);
 
    if (pipe(fd) < 0) {
        printf("Can\'t open pipe\n");
        exit(-1);
    }
 
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key.\n");
        exit(-1);
    }
 
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t create semaphore.\n");
        exit(-1);
    }
 
    result = fork();
 
    if (result < 0) {
        printf("Can\'t fork child.\n");
        exit(-1);
    }
    else if (result > 0) {
 
        /* Parent process */
 
        for (int i = 0; i < n; ++i) {
            size = write(fd[1], "Hello, world!", 14);
 
            if (size != 14) {
                printf("Parent can\'t write all string to pipe.\n");
                exit(-1);
            }
 
            mybuf.sem_num = 0;
            mybuf.sem_op = 2;
            mybuf.sem_flg = 0;
 
            if (semop(semid, &mybuf, 1) < 0) {
                printf("Parent can\'t A.\n");
                exit(-1);
            }

            mybuf.sem_num = 0;
            mybuf.sem_op = 0;
            mybuf.sem_flg = 0;
        
            if (semop(semid, &mybuf, 1) < 0) {
                printf("Parent can\'t Z.\n");
                exit(-1);
            }
        
            
 
            size = read(fd[0], resstring, 14);
 
            if (size < 0) {
                printf("Parent can\'t read string from pipe\n");
                exit(-1);
            }
 
            printf("Parent read the string: %s\n", resstring);
        }

        if (close(fd[0]) < 0) {
            printf("Parent can\'t close reading side of the pipe\n"); 
            exit(-1);
        }
        if (close(fd[1]) < 0) {
            printf("Parent can\'t close writing side of the pipe\n"); 
            exit(-1);
        }
 
 
        printf("Parent exit.\n");
 
    }
    else {
 
        /* Child process */

        for (int i = 0; i < n; ++i) {
 
            mybuf.sem_num = 0;
            mybuf.sem_op = -1;
            mybuf.sem_flg = 0;
 
            if (semop(semid, &mybuf, 1) < 0) {
                printf("Child can\'t D.\n");
                exit(-1);
            }
 
            size = read(fd[0], resstring, 14);
 
            if (size < 0) {
                printf("Child can\'t read the string from pipe.\n");
                exit(-1);
            }
 
            printf("Child read the string: %s\n", resstring);
        
            size = write(fd[1], "Hello, parent", 14);
        
            if (size != 14) {
                printf("Child can\'t write string to the pipe.\n");
                exit(-1);
            }    
 
            mybuf.sem_num = 0;
            mybuf.sem_op = -1;
            mybuf.sem_flg = 0;
 
            if (semop(semid, &mybuf, 1) < 0) {
                printf("Child can\'t D.\n");
                exit(-1);
            }
        }
        if (close(fd[0]) < 0) {
            printf("Child can\'t close reading side of the pipe\n"); 
            exit(-1);
        }
        if (close(fd[1]) < 0) {
            printf("Child can\'t close writing side of the pipe\n"); 
            exit(-1);
        }
        printf("Child exit.\n");
    }
 
    return 0;
}
