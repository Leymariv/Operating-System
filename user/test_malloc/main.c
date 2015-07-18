#include "mem.h"
#include <stdio.h>
#include <string.h>


int main(int argc, char * argv[])
{
	(void)argc;
	(void)argv;
	printf("1000\n");
	char * j = mem_alloc(1000);
	j[1000-1]='2';
	printf("val : %c\tAddr : %x\n",j[1000-1], (int)j);
	printf("500 non free\n");
	char *i = mem_alloc(500);
	i[500-1]='3';
	printf("val : %c\tAddr : %x\n",i[500-1], (int)i);
	mem_free(j,1000);	
	printf("500 free\n");
	i = mem_alloc(500);
	i[500-1]='3';
	printf("val : %c\tAddr : %x\n",i[500-1], (int)i);	
	mem_free(i, 500);
	printf("4000\n");
	i = mem_alloc(4000);
	i[4000-1]='4';
	printf("val : %c\tAddr : %x\n",i[4000-1], (int)i);
	mem_free(i, 4000);
	printf("8000\n");
	i = mem_alloc(8000);
	i[8000-1]='5';
	printf("val : %c\tAddr : %x\n",i[8000-1], (int)i);
	mem_free(i, 8000);
	printf("OK\n");
	return 0 ;
}
