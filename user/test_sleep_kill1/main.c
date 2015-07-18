#include <stdio.h>
#include <syscall.h>

int main(int argc, char *argv[])
{
	unsigned long quartz, ticks, freq;
	clock_settings(&quartz,&ticks);
	freq=quartz/ticks;
	(void)argv;
	unsigned long param = (unsigned long)argc;
	while(1)
	{
		wait_clock(param*freq+current_clock());
		switch(param){
		case 1: printf("."); break;
		case 2: printf("-"); break;
		case 5: printf("+"); break;
		case 10: printf("*"); break;
		default :break;
		}
	}
	return 0;
}
