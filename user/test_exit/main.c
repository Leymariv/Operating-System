#include <stdio.h>
#include <syscall.h>

int main(int argc, char *argv[])
{
	printf("*****************************\nTest exit\n"
	       "*****************************\n");
	(void)argc;
	(void)argv;
	unsigned long quartz, ticks, freq;
	clock_settings(&quartz,&ticks);
	freq=quartz/ticks;
	unsigned long param = 0 ;
	while(1)
	{
		printf("Je vais dormir %li secondes\n", param);
		wait_clock(param*freq + current_clock());
		if(param == 4)
		{
			printf("Exit");
			exit(0);
		}
		param ++;	
	}
	printf("\n*****************************\nTest fini\n"
	       "*****************************\n");
	return 0;
}
