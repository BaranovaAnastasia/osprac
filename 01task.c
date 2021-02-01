#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	pid_t pid, ppid;

	pid  = getpid();
	ppid = getppid();

	// С каждым новым вызовом значение pid будет 
	// увеличиваться (на единицу, если вызывать подряд).
	// Новый вызов порождает новый процесс, которому 
	// ОС пытается присвоить свободный номер больший, 
	// чем у процесса, созданного перед ним.

	printf("My pid = %d, my ppid = %d\n", (int)pid, (int)ppid);

	return 0;
}
