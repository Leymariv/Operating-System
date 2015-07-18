#include <stdio.h>
#include <syscall.h>
#include <string.h>

int main(int argc, char * argv[])
{
	(void)argc;
	(void)argv;
	printf("Je lance 3 programmes qui ne se terminent jamais\n");
	start("test_ps1", 3000, 3, (void*)1);
	start("test_ps1", 3000, 3, (void*)0);
	return 0 ;
}
