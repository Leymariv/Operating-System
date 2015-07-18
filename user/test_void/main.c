#include <stdio.h>
#include <syscall.h>

int main(int argc, char *argv[]){
	(void)argc;
	(void)argv;
	wait_clock(current_clock()+100);
	printf(" test_void1");
	return 0;
}
