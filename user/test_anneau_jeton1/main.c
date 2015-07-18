#include <stdio.h>
#include <syscall.h>

int main(int argc, char *argv[])
{
	(void)argv;
	int fid=(int)argc;
	unsigned long quartz, ticks, freq;
	clock_settings(&quartz,&ticks);
	freq=quartz/ticks;
	while (1){
		int jeton;
		printf("%i essaye de prendre le jeton\n",getpid());
		preceive(fid,&jeton);
		printf("%i a pris le jeton %i, "
"l'incremente et le garde un peu\n",
		       getpid(),
		       jeton++);
		wait_clock(3*freq+current_clock());
		printf("%i remet le jeton\n",getpid());
		psend(fid,jeton);
		wait_clock(1*freq+current_clock());
	}
	return 0;
}
