#include <stdio.h>
#include <syscall.h>

int main(int argc, char * argv[])
{
	(void)argc;	
	(void)argv;
	int sem1 = screate(0);
	int sem2 = screate(1);
	for(int i = 0; i<2; i++)
	{
		start("test_sinfo1", 1000, getprio(getpid()), (void*)sem1);
	}
	for(int i = 0; i<2; i++)
	{
		start("test_sinfo1", 1000, getprio(getpid()), (void*)sem2);
	}
	return 0 ;
}
