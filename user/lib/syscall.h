#ifndef __SYSCALL_H__
#define __SYSCALL_H__

typedef struct FILE_ {
	void *pt_elem;
	int indice;
	int type_file;
	int right;
} FILE;

void clock_settings(unsigned long *quartz, unsigned long *ticks);
int chprio(int pid, int newprio);
unsigned long current_clock();
void exit(int retval);
int getpid(void);
int getprio(int pid);
int kill(int pid);
int pcount(int fid, int *count);
int pcreate(int count);
int pdelete(int fid);
int preceive(int fid, int *message);
int preset(int fid);
int psend(int fid, int message);
int start(const char *name, unsigned long ssize, int prio, void *arg);
void wait_clock(unsigned long clock);
int waitpid(int pid, int *retvalp);
void *shm_create(const char *key);
void *shm_acquire(const char *key);
void shm_release(const char *key);
int scount(int sem);
int screate(short int count);
int sdelete(int sem);
int sreset(int sem,short int count);
int signal(int sem);
int signaln(int sem, short int count);
int try_wait(int sem);
int wait(int sem);
void cons_echo(int on);
unsigned long cons_read(char *string, unsigned long length);
int cons_write(const char *str, long size);
void reboot(void);
void ps(void);
void sinfo(void);
void pinfo(void);
FILE *fopen(const char *path, const char *mode);
int fclose(FILE *fp);
int fread(void *ptr, int size, int nmemb, FILE *stream);
int fwrite(const void *ptr, int size, int nmemb, FILE *stream);

#endif
