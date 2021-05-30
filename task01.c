#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

void moveNext(char *fileName, int i) {
    fileName[i] += 1;

    if (fileName[i] > 'z') {
        fileName[i] = 'a';
        moveNext(fileName, i - 1);
    }
}

int main() {
    int fd;
    int recursionDepth = 0;
    char fileName[] = "aa";
    char filePath[] = "dir/aa";
    (void) umask(0);


    if (mkdir("dir", 0777) < 0) {
        printf("Can't create dir \"dir\"");
        exit(-1);
    }


    if ((fd = open("dir/a", O_WRONLY | O_CREAT, 0666)) < 0) {
        printf("Can\'t open file \'a\'\n");
        exit(-1);
    }

    if (close(fd) < 0) {
        printf("Can\'t close file\n");
    }

    while (1) {

        moveNext(filePath, 5);

	if (recursionDepth) {
		if (symlink(fileName, filePath) != 0) {
            		printf("Can\'t create symlink\n");
            		exit(-1);
        	}		
	} else {
		char prev[] = "a";
		if (symlink(prev, filePath) != 0) {
            		printf("Can\'t create symlink\n");
            		exit(-1);
        	}		
	}

        if ((fd = open(filePath, O_WRONLY, 0666)) < 0) {
            break;
        } else {
            recursionDepth++;
        }

        if (close(fd) < 0) {
            printf("Can\'t close file\n");
        }

        moveNext(fileName, 1);
    }

    printf("Result recursion depth: %d\n", recursionDepth);

    return 0;
}
