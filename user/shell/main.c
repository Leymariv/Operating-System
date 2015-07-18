#include <stdio.h>
#include <syscall.h>
#include <string.h>
#include <syscall_gdf.h>
#include "readcmd.h"

#define SIZE_TAMPON 80-7

const char * prompt = "> ";
int buff_size = SIZE_TAMPON ;
char tampon[SIZE_TAMPON];

static void clean_tampon()
{
	for(int i=0; i<SIZE_TAMPON; i++)
		tampon[i]=0;
}

static int atoi( char* pStr ) 
{
  int iRetVal = 0; 
 
  if ( pStr )
  {
    while ( *pStr && *pStr <= '9' && *pStr >= '0' ) 
    {
      iRetVal = (iRetVal * 10) + (*pStr - '0');
      pStr++;
    }
  } 
  return iRetVal==0?-1:iRetVal; 
}

int main(int argc, char * argv[])
{
	(void)argc;
	(void)argv;
	unsigned long real_size ;	
	int echo = 1 ;

	while(1)
	{
		char dossier_courant[20];
		get_dcour(dossier_courant);
		printf("%s%s", dossier_courant,prompt);
		real_size = cons_read(tampon, (unsigned long)buff_size);
		if(echo == 0)
		{
			cons_write(tampon, (long)real_size);
			printf("\n");
		}
		if(real_size != 0)
		{
			struct cmdline * line = readcmd(tampon);
#if 0
			if(line != NULL)
			{
				printf("Nb arguments %i\n", line->nb_params);
				printf("Commande : %s\n", line->cmd[0]);
				printf("Taille commande : %i\n", strlen(line->cmd[0]));
				for(int j = 1; j <=  line->nb_params ; j++)
				{
					printf("Param %i : %s\n", j, line->cmd[j]);
				}
			}
#endif

			if (line->cmd[0]!=NULL){
			// COMMANDE PS
			if(!strcmp("ps", line->cmd[0]))
			{
				if(line->nb_params == 0)		
				{
					ps();
				} else {
					printf("ps : too many arguments\n");	
				}


			// COMMANDE LS
			} else if(!strcmp("ls", line->cmd[0]))
			{
				if(line->nb_params > 1)		
				{
					printf("ls : too many arguments\n");	
				} else if(line->nb_params == 0){
					ls_gdf(NULL);	
				} else if(ls_gdf(line->cmd[1])==-1){
					printf("ls: unknown directory named \"%s\"\n", line->cmd[1]);
				}
				

			// COMMANDE CD
			} else if(!strcmp("cd", line->cmd[0])){
				if(line->nb_params > 1)
					printf("cd : too many arguments\n");	
				else if(line->nb_params == 0)
					cd_gdf(NULL);
				else if (cd_gdf(line->cmd[1])==-1)
					printf("cd: unknown directory named \"%s\"\n", line->cmd[1]);


			// COMMANDE MKDIR
			} else if(!strcmp("mkdir", line->cmd[0])){
				if(line->nb_params == 0)
				{
					printf("[Usage] : mkdir <dir_name>*\n");
				} else {
					for(int i = 1; i<=line->nb_params; i++)
					{
						int ret=mkdir_gdf(line->cmd[i]);
						if (ret==-1)
							printf("mkdir: directory name \"%s\" already useor invalide\n", line->cmd[i]);
					}
				}

			
			// COMMANDE RMDIR
			} else if(!strcmp("rmdir", line->cmd[0])){
				if(line->nb_params == 0)
				{
					printf("[Usage] : rmdir <dir_name>*\n");
				} else {
					for(int i = 1; i<=line->nb_params; i++)
					{
						int ret=rmdir_gdf(line->cmd[i]);
						if (ret==-1)
							printf("rmdir: unknown directory \"%s\"\n", line->cmd[i]);
						else if (ret==-2)
							printf("rmdir: directory \"%s\" not empty\n", line->cmd[i]);
					}
				}


			// COMMANDE TOUCH
			} else if(!strcmp("touch", line->cmd[0])){
				if(line->nb_params == 0)
				{
					printf("[Usage] : touch <file_name>*\n");
				} else {
					for(int i = 1; i<=line->nb_params; i++)
					{
						int ret=touch_gdf(line->cmd[i]);
						if (ret==-1)
							printf("touch: file name \"%s\" already used or invalide\n", line->cmd[i]);
					}
				}


			// COMMANDE RM
			} else if(!strcmp("rm", line->cmd[0])){
				if(line->nb_params == 0)
				{
					printf("[Usage] : rm <file_name>*\n");
				} else {
					for(int i = 1; i<=line->nb_params; i++)
					{
						int ret=rm_gdf(line->cmd[i]);
						if (ret==-1)
							printf("rm: unknown file \"%s\"\n", line->cmd[i]);
						else if (ret==-2)
							printf("rm: file \"%s\" not empty\n", line->cmd[i]);
					}
				}


			// COMMANDE ECHO

			} else if(!strcmp("echo", line->cmd[0])){
				if(line->nb_params != 0)
				{
					printf("[Usage] : echo\n");
				} else {
					echo = echo==1?0:1;
					cons_echo(echo);
				}

			// COMMANDE CAT

			} else if(!strcmp("cat", line->cmd[0])){
				if(line->nb_params != 1)
				{
					printf("[Usage] : cat <file_name>\n");
				} else {
					if (cat_gdf(line->cmd[1])==-1)
						printf("Unknown file name \"%s\"\n", line->cmd[1]);
				}

			// COMMANDE EDIT
			
			} else if(!strcmp("edit", line->cmd[0])){
				if(line->nb_params != 1)
				{
					printf("[Usage] : edit <file_name>\n");
				} else {
					void *p=shm_create("name_file");
					strcpy((char *)p,line->cmd[1]);
					int pid=start("editor",4000,220,NULL);
					int ret;
					waitpid(pid,&ret);
					shm_release("name_file");
					if (ret==-1)
						printf("unknown file \"%s\"\n", line->cmd[1]);
				}


			// COMMANDE TEST

			} else if(!strcmp("test", line->cmd[0])){
				if(line->nb_params != 1)
				{
					printf("[Usage] : test <test_name>\n");
				} else {
					int pid ;
					if ((pid=start(line->cmd[1],4000,getprio(getpid()),NULL))==-1)
						printf("test: unknown test \"%s\"\n", line->cmd[1]);
					else
						waitpid(pid,0);
				}


			// COMMANDE EXIT
			} else if(!strcmp("exit", line->cmd[0])){
				if(line->nb_params != 0)
				{
					printf("[Usage] : exit\n");
				} else {
					reboot();
				}

			// COMMANDE KILL
			} else if(!strcmp("kill", line->cmd[0])){
				if(line->nb_params == 0)
				{
					printf("[Usage] : kill <pid>*\n");
				} else {
					for(int i = 1; i<=line->nb_params; i++)
					{
						int pid ;
						if((pid = atoi((char*)(line->cmd[i]))) != -1)
						{
							if(kill(pid)==-1)
							{
								printf("kill (%i): aucun processus de ce pid\n", pid);
							}
					
						} else {
							printf("kill: argument \"%s\" invalide\n", line->cmd[i]);
						}
					}
				}


			// COMMANDE SINFO
			} else if(!strcmp("sinfo", line->cmd[0])){
				if(line->nb_params != 0)
				{
					printf("[Usage] : sinfo\n");
				} else {
					sinfo();
				}


			// COMMANDE PINFO
			} else if(!strcmp("pinfo", line->cmd[0])){
				if(line->nb_params != 0)
				{
					printf("[Usage] : pinfo\n");
				} else {
					pinfo();
				}


			// COMMANDE CLEAR
			} else if(!strcmp("clear", line->cmd[0])){
				if(line->nb_params != 0)
				{
					printf("[Usage] : clear\n");
				} else {
					printf("\f");
				}
				
			// COMMANDE NICE
			} else if(!strcmp("nice", line->cmd[0])){
				if(line->nb_params != 2)
				{
					printf("[Usage] : nice <pid> <new_prio>\n");
				} else {
					int pid_nice = atoi(line->cmd[1]);
					int prio_nice = atoi(line->cmd[2]);
					if(prio_nice != -1 && pid_nice != -1)
					{
						if(chprio(pid_nice, prio_nice) == -1)	
						{
							printf("nice: bad arguments\n");
						}
					} else {
						printf("nice: bad arguments\n");
						printf("[Usage] : nice <pid> <new_prio>\n");
					}
				}

			// COMMANDE HELP
			} else if(!strcmp("help", line->cmd[0])){
				if(line->nb_params != 0)
					printf("[Usage] : help\n");
				else {
					FILE *f=fopen("~/help/help","r");
					if ((int)f==1)
						printf("Il ne fallait pas supprimer le fichier help\n");
					else if ((int)f==2)
						printf("Il ne fallait pas modifier le fichier help\n");
					else{
						char tamp='w';
						while (tamp!=0){
							fread(&tamp,sizeof(char),1,f);
							printf("%c",tamp,(int)tamp);
						}
						printf("\b");
						fclose(f);
					}
				}
			} else {
				printf("Unknown command \"%s\"\n", line->cmd[0]);
			}
			} 
			free_cmd(line);
			
		}
		clean_tampon();
	}
	return 0 ;
}
