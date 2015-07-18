#include <stdio.h>
#include <syscall.h>

int main(int argc, char *argv[])
{
	printf("*****************************\nTest sem\n"
	       "*****************************\n");
	(void)argv;
	(void)argc;
	int num_sem ;
	int pids[5] ;
	if((num_sem = screate(1)) == -1)
	{
		printf("Echec lors de la creation du semaphore.\n");
	}
	printf("Semaphore %i initialise a : %i\n", num_sem, scount(num_sem));
	// on cree des process qui vont utiliser le semaphore
	for(int i = 0 ; i< 3; i++)
	{
		pids[i] = start("test_sem4", 10000, 1, (void *)num_sem);
	}
	
	int val ;
	for(int i = 0 ; i< 3; i++)
	{
		printf("pid numero %i : %i\n", i, pids[i]);
		waitpid(pids[i], &val);
	}
	printf("Valeur du semaphore %i a la fin du test : %i\n", num_sem, scount(num_sem));
	printf("\n*****************************\nTest fini\n"
	       "*****************************\n");
	return 0;
}
