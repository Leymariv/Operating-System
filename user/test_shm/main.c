#include <stdio.h>
#include <syscall.h>
#include <stddef.h>

int main(int argc, char *argv[])
{
	printf("*****************************\nTest shm\n"
	       "*****************************\n");
	(void)argc;
	(void)argv;
	
	const char *c="page_test_shm";
	int fid=pcreate(1);
	int ret;
	int pid=start("test_shm1",10000,getprio(getpid()),(void *)fid);
	void *pagep = shm_create(c);
	assert(pagep!=NULL);
	*((int*)pagep+4)=23;
	psend(fid, 1);
	preceive(fid,&ret);
	printf("[process %i] : %i\n",getpid(),*((int*)pagep+4));
	pdelete(fid);
	shm_release(c);
	waitpid(pid,&ret);
	assert(shm_acquire(c)==NULL);
	printf("\n*****************************\nTest fini\n"
	       "*****************************\n");
	return 0;
}
