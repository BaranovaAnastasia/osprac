#include <sys/types.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 

int main() { 
	int fd[2], fd1[2], result;
	size_t size; 
	char string[] = "Hello, world!"; 
	char string1[] = "!dlrow ,olleH"; 
	char resstring[14]; 
	char resstring1[14]; 

	if (pipe(fd) < 0) {
    		printf("Can\'t open pipe 1\n");
    		exit(-1);
  	}
	if (pipe(fd1) < 0) {
    		printf("Can\'t open pipe 2\n");
    		exit(-1);
  	} 

	result = fork();
 
	if(result < 0) { 
		printf("Can\'t fork child\n"); 
    		exit(-1);
	} 
	else if (result > 0) { 
		if (close(fd[0]) < 0) {
        		printf("parent: Can\'t close reading side of pipe\n"); 
			exit(-1);
    		}
 		
		printf("Parent writting string...(pipe 1)\n");
		size = write(fd[1], string, 14);  
		if (size != 14) {
      			printf("Can\'t write all string to pipe\n");
      			exit(-1);
    		}

    		if (close(fd[1]) < 0) {
      			printf("parent: Can\'t close writing side of pipe\n"); 
			exit(-1);
    		} 
		printf("Parent exit\n"); 
	} 
	else { 
		if (close(fd1[0]) < 0) {
        		printf("child: Can\'t close reading side of pipe\n"); 
			exit(-1);
    		} 
		if (close(fd[1]) < 0) {
         		printf("child: Can\'t close writing side of pipe\n"); 
			exit(-1);
    		} 

		printf("Child reading string...(pipe 1)\n");
		size = read(fd[0], resstring, 14); 
		if (size < 0) {
      			printf("Can\'t read string from pipe\n");
      			exit(-1);
    		}

    		printf("Child read: %s\n", resstring);

    		if (close(fd[0]) < 0) {
      			printf("child: Can\'t close reading side of pipe\n"); 
			exit(-1);
    		}
	
 		printf("Child writting string...(pipe 2)\n");
		size = write(fd1[1], string1, 14);  
		if (size != 14) {
      			printf("Can\'t write all string to pipe\n");
      			exit(-1);
    		} 
		if (close(fd1[1]) < 0) {
      			printf("child: Can\'t close writing side of pipe\n"); 
			exit(-1);
    		}
		printf("Child exit\n"); 
	} 
 
	sleep(3); 
	if (result > 0) { 
		if (close(fd1[1]) < 0) {
      			printf("parent: Can\'t close writing side of pipe\n"); 
			exit(-1);
    		}
		
		printf("Parent reading string...(pipe 2)\n");
		size = read(fd1[0], resstring1, 14);
     		if (size < 0) {
      			printf("Can\'t read string from pipe\n");
      			exit(-1);
    		} 
		printf("Parent read: %s\n", resstring1); 

		if (close(fd1[0]) < 0) {
      			printf("parent: Can\'t close reading side of pipe\n"); 
			exit(-1);
    		}
		printf("Parent exit\n"); 
	} 
 
	return 0; 
}
