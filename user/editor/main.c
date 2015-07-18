#include <syscall.h>
#include <syscall_editor.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	char *name=(char *)shm_acquire("name_file");
	if (editor_on(name)==-1){
		shm_release("name_file");
		return -1;
	}
	while(1){
		char tampon[80];
		long unsigned l=cons_read(tampon,(unsigned long)80);
		if (tampon[l-1]==27)
			break;
	}
	editor_off();
	shm_release("name_file");
	return 0;
}
