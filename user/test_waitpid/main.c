#include <stdio.h>
#include <syscall.h>

int main(int argc, char *argv[])
{
	printf("*****************************\nTest waitpid\n"
	       "*****************************\n");
	(void)argc ;
	(void)argv;
	unsigned long quartz, ticks, freq;
	clock_settings(&quartz,&ticks);
	freq=quartz/ticks;
	int i = 1 ;
	int pid = start("test_waitpid1", 512, getprio(getpid()), (void *)i);
	wait_clock(3*freq+current_clock());	
	printf("J'attends mon fils de pid %i.\n", pid);
	int j ;
	assert(waitpid(pid, &j) == pid);
	printf("Mon fils est de retour ! Sa valeur de retour est %i.\n", j);

	int pid2 = start("test_waitpid1", 512, getprio(getpid()), (void *)i);
	i++;
	start("test_waitpid1", 512, getprio(getpid()), (void *)i);
	wait_clock(3*freq + current_clock());	
	printf("J'attends n'importe lequel de mes fils.\n");
	assert(waitpid(-1, &j)==pid2);
	printf("Un de mes fils est de retour ! Sa valeur de retour est %i.\n", 
	       j);
	printf("\n*****************************\nTest fini\n"
	       "*****************************\n");
	return 0;
}
