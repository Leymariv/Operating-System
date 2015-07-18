#include <stdio.h>
#include <syscall.h>

int main(int argc, char * argv[])
{	
	(void)argv;
	wait(argc);
	while(1);
	return 0 ;
}
