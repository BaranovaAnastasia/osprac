// В начале семафор инициализирован 0. Родитель создает ребенка, 
// который делает D(S, 1), ждет, пока родитель запишет сообщение.
// Родитель производит запись и сообщает об этом ребенку, используя A(S, 2)
// и блокируясь через Z(S). Запустившийся ребенок меняет значение семафора на 1,
// читает данные из pipe и записывает, после чего снова выполняет D(S, 1), чтобы
// запустить родителя и D(S, 1), чтобы дождаться завершения работы родителя.
// Запустившийся родитель читает данные, записанные ребенком, и сам производит запись.
// Процесс повторяется n раз.

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
 
    int n;
    printf("n = ");
    scanf("%d", &n);
 
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
            } else {
                printf("Parent wrote string to pipe.\n");
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
            } else {
                printf("Child wrote string to pipe.\n");
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
