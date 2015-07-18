#include <stdio.h>
#include <syscall.h>
#include <string.h>

int main(int argc, char * argv[])
{
	(void)argv;
	if(argc == 0)
	{
		while(1);
	} else if(argc == 1){
		while(1)
		{
			wait_clock(current_clock() + 10 * 50);
		}
	}
	return 0 ;
}
