#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
	printf("Аргументы командной строки:\n");
	for (int i = 0; i < argc; ++i) {
		printf("%s\n", argv[i]);
	}
	
	printf("Параметры окружающей среды процесса:\n");
	for (int i = 0; envp[i] != NULL; ++i) {
		printf("%s\n", envp[i]);
	}
	
	return 0;
}
