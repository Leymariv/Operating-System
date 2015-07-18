#include <stdio.h>

int main(int argc, char *argv[])
{
	(void)argv;
	if (argc==1)
		*((int*)0x142800)=0x1;
	else
		*((int*)0xc0000000)=0x1;
	printf("\nJ'ai réussi a foutre la merde !\n");
	return 1;
}
