#include <stdio.h>
#include <syscall.h>
#include <stddef.h>

int main(int argc, char *argv[])
{
	(void)argv;
	int fid=(int)argc;
	
	const char *c="page_test_shm";
	int ret;
	preceive(fid,&ret);
	void *pagep=shm_acquire(c);
	assert(pagep!=NULL);
	printf("[process %i] : %i\n",getpid(),*((int*)pagep+4));
	*((int*)pagep+4)=54;
	psend(fid,3);
	shm_release(c);
	return 0;
}
