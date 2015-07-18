#include <stdio.h>
#include <syscall.h>

int main(int argc, char *argv[])
{
	unsigned long quartz, ticks, freq;
	clock_settings(&quartz,&ticks);
	freq=quartz/ticks;
	(void)argv;
	unsigned long param = (unsigned long)argc;
	int i = 0 ;
	while(i < 10)
	{
		wait_clock(param*freq+current_clock());
		printf(".");	
		i++ ;
	}
	printf("[%i] Termine.\n", getpid());
	param = 2 ;
	return (int)param;
}
