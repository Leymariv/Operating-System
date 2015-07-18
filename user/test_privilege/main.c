#include <stdio.h>
#include <syscall.h>

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	printf("*****************************\nTest privilege\n"
	       "*****************************\n");
	assert(getprio(getpid())==128);
	printf("1");
	int ret;
	int pid=start("test_privilege1",4000,127,(void*)1);
	assert(waitpid(pid,&ret)==pid);
	assert(ret==0);
	printf(" 2");
	pid=start("test_privilege1",4000,127,(void*)2);
	assert(waitpid(pid,&ret)==pid);
	assert(ret==0);
	printf(" 3.\n");
	printf("\n*****************************\nTest fini\n"
	       "*****************************\n");
	return 0;
}
