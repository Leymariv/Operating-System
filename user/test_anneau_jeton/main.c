#include <stdio.h>
#include <syscall.h>

int main(int argc, char *argv[])
{
	printf("*****************************\nTest anneau a jeton\n"
	       "*****************************\n");
	(void)argc;
	(void)argv;
	unsigned long quartz, ticks, freq;
	clock_settings(&quartz,&ticks);
	freq=quartz/ticks;
	int fid=pcreate(10);
	int jeton=1;
	psend(fid,jeton);
	int pid1 = start("test_anneau_jeton1", 512, getprio(getpid()), (void *)fid);
	wait_clock(1);
	int pid2 = start("test_anneau_jeton1", 512, getprio(getpid()), (void *)fid);
	wait_clock(1);
	int pid3 = start("test_anneau_jeton1", 512, getprio(getpid()), (void *)fid);
	wait_clock(10*freq+current_clock());
	kill(pid1);
	kill(pid2);
	kill(pid3);
	printf("\n*****************************\nTest fini\n"
	       "*****************************\n");
	return 0;
}
