#include <stdio.h>
#include <syscall.h>

int main(int argc, char *argv[])
{
	(void)argv ;
	int num_sem = (int)argc;
	printf("[Process %i] : Valeur du semaphore avant wait: %i.\n", getpid(), scount(num_sem));
	if(wait(num_sem) != 0)
	{
		printf("Erreur wait !\n");
		return -1 ;
	}
	printf("[Process %i] J'ai la main.\n", getpid());
	if(signal(num_sem) != 0)
	{
		printf("Erreur signal !\n");
		return -1 ;
	}
	printf("[Process %i] : Valeur du semaphore apres signal: %i.\n",getpid(),  scount(num_sem));
	return 0;
}
