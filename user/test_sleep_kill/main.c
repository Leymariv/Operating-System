#include <stdio.h>
#include <syscall.h>

int main(int argc, char *argv[])
{
	printf("*****************************\nTest sleep & kill\n"
	       "*****************************\n");
	(void)argc;
	(void)argv;
	unsigned long quartz, ticks, freq;
	clock_settings(&quartz,&ticks);
	freq=quartz/ticks;
	int i = 1 ;
	int pid1 = start("test_sleep_kill1", 512, getprio(getpid()), (void *)i);
	int j = 2 ;
	int pid2 = start("test_sleep_kill1", 512, getprio(getpid()), (void *)j);
	int k = 5 ;
	int pid3 = start("test_sleep_kill1", 512, getprio(getpid()), (void *)k);	
	int l = 10 ;
	int pid4 = start("test_sleep_kill1", 512, getprio(getpid()), (void *)l);
	wait_clock(20*freq+current_clock());
	kill(pid1);
	kill(pid2);
	kill(pid3);
	kill(pid4);
	printf("\n*****************************\nTest fini\n"
	       "*****************************\n");
	return 0;
}
