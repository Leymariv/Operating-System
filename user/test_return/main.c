#include <stdio.h>
#include <syscall.h>

int main(int argc, char *argv[])
{
	printf("*****************************\nTest return\n"
	       "*****************************\n");
	(void)argc;
	(void)argv;
	int i = 0;
	int pid = start("test_return1", 512, getprio(getpid()), (void *)i);
	int j ;
	waitpid(pid, &j);
	printf("Valeur retournee : %i\n", j);
	printf("\n*****************************\nTest fini\n"
	       "*****************************\n");
	return 0;
}
