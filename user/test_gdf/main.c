#include <syscall.h>
#include <stdio.h>
#include <stddef.h>

int main(int argc, char * argv[])
{
	(void)argc;
	(void)argv;
	printf("%i-",(int)(fopen("../b/","r")));
	printf("%i\n",(int)(fopen("../~/b","r")));
	return 0;
}
