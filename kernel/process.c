#include "process.h"
#include "mem.h"
#include "traitement_IT.h"
#include "cpu.h"
#include <stdio.h>
#include "debugger.h"
#include "message_file.h"
#include "semaphore.h"
#include "hash.h"
#include "userspace_apps.h"
#include "alloc_physique.h"
#include <string.h>
#include "lifo.h"
#include <stddef.h>

#define ESP0_TSS 0x20004
#define CR3 0x2001C
#define GB3 0xc0000000 // 3Go
#define GB2 0x80000000 // 2Go
#define GB1 0x40000000  // 1Go
#define MB512 0x20000000 // 512Mo
#define MB256 0x10000000 // 256 Mo

#define ADDR_VALIDE(addr) (unsigned)addr>=0x40000000

/* Table des symbols d'application */
hash_t table_symbols;

void init_table_symbols(void)
{
	hash_init_string(&table_symbols);
	int i=0;
	const struct uapps *cour=&symbols_table[i];
	while (cour->name!=NULL){
		hash_set(&table_symbols,(void*)cour->name,(void*)cour);
		cour=&symbols_table[++i];
	}
}

int free_directory(int *** paged)
{
	for (int i=64; i<1024; i++){
		if ( ((int)(paged[i]) & 0x1)!=0){
			for (int j=0; j<1024; j++){
				if ((((int *)((int)(paged[i]) & 
					      0xfffff000))[j] & 0x1)!=0 ){
					pfree(((int **)((int)(paged[i]) & 
							0xfffff000))[j]);
				}
			}
			pfree((int*)((int)(paged[i]) & 0xfffff000));
			paged[i]=0;
		}
	}
	pfree((int*)paged);
	return -1;
}

/*table des processus*/
process* table_process[NBPROC];
link list_pid_dispo;

/* pid du processus courant*/
int pid_current_process;

/*liste des processus activable*/
link list_process_activable;

/*liste des processus endormis triée par heure de reveil*/
link list_process_endormi ;

extern void launch_user_mode();

extern void ctx_sw(int *, int*);

static int kill_rec(process * proc)
{
	proc->pere=NULL;
	proc->etat=MORT;
	lifo_add(&list_pid_dispo,proc->pid);
	process * cour;
	queue_for_each(cour,&proc->list_fils,process,fils_suiv){
		if (cour->etat==ZOMBIE)
			kill_rec(cour);
	}    
	while (!queue_empty(&proc->list_fils))
		cour=(process *)queue_out(&proc->list_fils,process,fils_suiv);
	return 0 ;
}

/* 
 * proc pointeur vers le processus venant de se terminer, 
 * d'être killé ou de s'être exit
 */
static void check_father(process * proc)
{
	process * cour;
	queue_for_each(cour,&proc->list_fils,process,fils_suiv){
		if (cour->etat==ZOMBIE)
			kill_rec(cour);
	}
	while (!queue_empty(&proc->list_fils))
		cour=(process *)queue_out(&proc->list_fils,process,fils_suiv);
	if(proc->pere->etat == BLOQUE_FILS && 
	   (proc->pere->attente == 0 || proc->pere->attente == proc->pid))
	{
		proc->pere->buff_pid = proc->pid ;
		proc->pere->etat = ACTIVABLE ;
		queue_add(proc->pere, 
			  &list_process_activable, 
			  process, 
			  suiv, 
			  prio);
	}
}

process *bloque_process(int num)
{
	table_process[pid_current_process]->etat=BLOQUE_IO;
	table_process[pid_current_process]->id_fs=num;
	return table_process[pid_current_process];
}

process *bloque_process_sem(int sem)
{
	table_process[pid_current_process]->etat=BLOQUE_SEM;
	table_process[pid_current_process]->id_fs=sem;
	return table_process[pid_current_process];
}

void debloque_process(int pid)
{
	table_process[pid]->etat=ACTIVABLE;
	queue_add(table_process[pid], 
		  &list_process_activable, 
		  process, 
		  suiv, 
		  prio);
}

int start(const char *name, unsigned long ssize, int prio, void *arg)
{
        /* On recupere les info sur le process et on retourne -1 
	   s'il n'existe pas */
	struct uapps * sym=hash_get(&table_symbols,(void *)name,(void *)NULL); 
	if (sym==NULL)
	{
		return -1 ;
	}
	if (prio<1 || prio >MAXPRIO || ssize > 0x400000)
	{	
		return -1;
	}
	if (lifo_empty(&list_pid_dispo))
	{
		return -1;
	}
	int i=lifo_out(&list_pid_dispo);
	if (table_process[i]==NULL)
		table_process[i]=(process*)mem_alloc(sizeof(process));

        /* On creer la page directory dans l'espace utilisateur (64Mo-256Mo) 
	   si elle n'existe pas encore*/
	if (table_process[i]->PAGED!=0) 
		free_directory((int***)table_process[i]->PAGED);
	palloc((int**)&table_process[i]->PAGED);
        // Astuce : Copy de la partie kernel, que l'on peut réutiliser
	for (int k=0; k<64 ; k++) 
		((int*)table_process[i]->PAGED)[k]=
			((int*)table_process[0]->PAGED)[k];

	/* On alloue une page pour la pile kernel */
	int *tmp_esp0;
	if (palloc(&tmp_esp0)==-1){
		lifo_add(&list_pid_dispo,i);
		return -1;
	}
	if (pmap((int***)table_process[i]->PAGED,
		 tmp_esp0,
		 ( int*)(MB512),3)==-1)
	{
		lifo_add(&list_pid_dispo,i);
		pfree(tmp_esp0);
	}
	table_process[i]->ESP0=(int)(MB512 + KB4 - 1*sizeof(int));
	tmp_esp0[KB4/sizeof(int)-1]=(int)launch_user_mode;
	
	int * pile_base;
        /* On initialise la pile a partir de PILE user (0x80000000-2Go) 
	   en sens decroissant */
	for (long k=0; k*KB4<=(long)ssize; k++){
		int *tmp;
		if (palloc(&tmp)==-1){
			lifo_add(&list_pid_dispo,i);
			return free_directory((int***)table_process[i]->PAGED);
		}
		if (k==0) // On memorise la base de la pile pour plus tard
			pile_base=tmp;
		if (pmap((int***)table_process[i]->PAGED,
			 tmp,
			 ( int*)(GB2-k*KB4),7)==-1)
		{
			lifo_add(&list_pid_dispo,i);
			pfree(tmp);
			return free_directory((int***)table_process[i]->PAGED);
		}
	}
	
	// On initialise le code, les données et le tas à 0x40000000-1Go
	int k;
	for (k=0; k*KB4<((int)(sym->end) - (int)(sym->start)); k++){ 
		int *tmp;
		if (palloc(&tmp)==-1){
			lifo_add(&list_pid_dispo,i);
			return free_directory((int***)table_process[i]->PAGED);
		}
		if (pmap((int***)table_process[i]->PAGED,
			 tmp,
			 (int *)(GB1+k*KB4),7)==-1)
		{
			lifo_add(&list_pid_dispo,i);
			pfree(tmp);
			return free_directory((int***)table_process[i]->PAGED);
		}
		if (pmap((int***)table_process[pid_current_process]->PAGED,
			 tmp,(int *)(MB256+k*KB4),7)==-1)
		{
			lifo_add(&list_pid_dispo,i);
			pfree(tmp);
			return free_directory((int***)table_process[i]->PAGED);
		}
	}
	memcpy((int*)MB256,sym->start,((int)(sym->end) - (int)(sym->start)));

	table_process[i]->heap_end = (char*)(GB1+(k)*KB4);
	table_process[i]->current = (char*)table_process[i]->heap_end ;

	pile_base[KB4/sizeof(int)-1]=(int)arg ;

	pile_base[KB4/sizeof(int)-2]=GB1;
	table_process[i]->ESP = (int)(MB512 + KB4 - 1*sizeof(int));
			
	//filiation
	INIT_LIST_HEAD(&table_process[i]->list_fils);
	process * pere = table_process[pid_current_process];
	table_process[i]->pere = pere;
	if (table_process[i]->fils_suiv.next!=NULL)
		queue_del(table_process[i],fils_suiv);
	queue_add(table_process[i], &pere->list_fils, process, fils_suiv, prio);

	table_process[i]->etat = ACTIVABLE ;
	table_process[i]->pid = i ;
	table_process[i]->hreveil = 0 ;
	table_process[i]->name = mem_alloc(strlen(name) *sizeof(char) + 1) ;
	strcpy((char*)table_process[i]->name, (char *)name);
	table_process[i]->prio = prio ;
	table_process[i]->buff_pid=0;
	table_process[i]->buff_val=0;
	table_process[i]->id_fs=0;
	table_process[i]->attente=0;

	queue_add(table_process[i],&list_process_activable,process,suiv,prio);
	scheduler();
	return i ;
}
	
int getpid(void)
{
	return table_process[pid_current_process]->pid;
}

void init_scheduler(void)
{
	table_process[0]=(process*)mem_alloc(sizeof(process));
	table_process[0]->etat = ACTIVABLE ;
	table_process[0]->pid = 0 ;
	table_process[0]->hreveil = 0 ;
	table_process[0]->name = "idle" ;
	table_process[0]->prio = 0 ;
	table_process[0]->PAGED=(*(int *)CR3);
	table_process[0]->ESP0=(*(int *)ESP0_TSS);
	pid_current_process=0;
	INIT_LIST_HEAD(&table_process[0]->list_fils);
	INIT_LIST_HEAD(&list_process_activable);
	INIT_LIST_HEAD(&list_process_endormi);
	lifo_init(&list_pid_dispo);
	for (int i=NBPROC-1; i>=1; i--)
		lifo_add(&list_pid_dispo,i);
}

void scheduler_process(int pid)
{
	process* current_process=table_process[pid_current_process];
	queue_add(current_process,&list_process_activable,process,suiv,prio);
	process* new_process = table_process[pid];
	queue_del(new_process,suiv);
	pid_current_process = pid;
	ctx_sw(current_process->regs, new_process->regs);
}

void scheduler(void)
{
	process* current_process=table_process[pid_current_process];
	/* si le processus courant doit être endormi*/
	if(current_process->etat == ENDORMI){
		queue_add(current_process, &list_process_endormi, 
			  process, suiv, hreveil);
	}else if(current_process->etat == ACTIVABLE){
		queue_add(current_process,
			  &list_process_activable,process,suiv,prio);
	}	
	/* on passe en activable les processus endormi dont 
	   l'heure de reveil est atteinte */
	process * p_wakeup = queue_bottom(&list_process_endormi,process,suiv);	
	while(p_wakeup != NULL && p_wakeup->hreveil <= current_clock())
	{
		queue_del(p_wakeup, suiv);
		queue_add(p_wakeup, &list_process_activable, 
			  process, suiv, prio);
		p_wakeup = queue_bottom(&list_process_endormi,process,suiv);	
	}
	/* election du nouveau processus actif */
	process* new_process = queue_out(&list_process_activable,process,suiv);	
	pid_current_process = new_process->pid;
#if 0
	if (new_process->pid!=0 || current_process->pid!=0){
		printf("| Schedule %i-%s |\t",new_process->pid,new_process->name);
	}
#endif	
	ctx_sw(current_process->regs, new_process->regs);
}

int chprio(int pid, int newprio)
{
	if(pid < 1 || pid > NBPROC - 1 || newprio > MAXPRIO || newprio < 1)
		return -1 ;
	process * p = table_process[pid];
	if(p == NULL)
		return -1 ;
	int oldprio = p->prio ;
	p->prio = newprio ;
	switch(p->etat)
	{
	case ACTIVABLE :
		// seul la liste des activables est trié par priorité
		//mise à jour de la file
		if (p->pid!=pid_current_process){
			queue_del(p, suiv);
			queue_add(p, &list_process_activable, 
				  process, suiv, prio);
		}
		break ;
	case BLOQUE_IO :
		queue_del(p,suiv);
		if (p->id_fs>0)
			queue_add(p,&table_file[p->id_fs-1]->receive,
				  process,suiv,prio);
		else 
			queue_add(p,&table_file[-(p->id_fs+1)]->send,
			process,suiv,prio);
		break;
	case BLOQUE_SEM : 
		queue_del(p,suiv);
		queue_add(p,&table_sem[p->id_fs]->process_bloque_sem,
				  process,suiv,prio);
		break ;
	case ENDORMI : break ;
	default : return -1;
	}
	scheduler();
	return oldprio ;
}

int getprio(int pid)
{
	if(pid < 0 || pid > NBPROC -1)
		return -1 ;
	process * proc = table_process[pid];
	if(proc == NULL || proc->etat == MORT || proc->etat == ZOMBIE)
		return -1 ;
	else 
		return proc->prio ;
}

void wait_clock(unsigned long clock)
{
	process * p = table_process[pid_current_process];
	p->hreveil = clock ;
	p->etat = ENDORMI ;
	scheduler();
}

int kill(int pid)
{
	if(pid < 1 || pid > NBPROC -1)
		return -1 ;
	process * proc = table_process[pid] ;
	if(proc == NULL || proc->etat==MORT || proc->etat==ZOMBIE)
     		return -1 ;
	// on supprime le processus de toutes les files
	if ((proc->etat==ACTIVABLE && 
	     pid != table_process[pid_current_process]->pid) || 
	    proc->etat==ENDORMI)
		queue_del(proc, suiv);
	else if (proc->etat==BLOQUE_IO)
		queue_del(proc,suiv);
	else if (proc->etat == BLOQUE_SEM){
		table_sem[proc->id_fs]->count ++ ;
		queue_del(proc,suiv);
	}
	if (proc->pere->etat==MORT || 
	    proc->pere->etat==ZOMBIE || proc->pere->pid==0){
		proc->etat = MORT ;
		lifo_add(&list_pid_dispo,proc->pid);
	} else {
		proc->etat = ZOMBIE;
	}
	check_father(proc);
	scheduler();
	return 0 ;
}

void exit(int retval)
{
	process * current_process = table_process[pid_current_process];
	if (current_process->pere->pid==0){
		current_process->etat = MORT ;
		lifo_add(&list_pid_dispo,current_process->pid);
		queue_del(current_process, fils_suiv);
		scheduler();
	}
	current_process->etat = ZOMBIE ;
	current_process->buff_val = retval ;
	check_father(current_process);
	scheduler();
	//no return
	exit(0);
}

int waitpid(int pid, int *retvalp)
{
	process * current_process = table_process[pid_current_process];
	process * fils ;
	int ret_valide=1;
	if(pid < 0)
	{
		int existfils=0;
		//on attends la terminaison de n'importe quel fils
		queue_for_each(fils, &current_process->list_fils, 
			       process, fils_suiv){
			if(fils->etat == ZOMBIE)
			{
				if(ADDR_VALIDE(retvalp))
					*retvalp = fils->buff_val ;
				else if (retvalp!=NULL)
					ret_valide=0;
				queue_del(fils,fils_suiv);
				fils->etat = MORT ;
				lifo_add(&list_pid_dispo, fils->pid);
				return ret_valide==1?fils->pid:-1 ;	
			} else if(fils->etat!=MORT) {
				existfils=1;
			}
		}
		if (!existfils)
			return -1;
		current_process->etat = BLOQUE_FILS ;
		current_process->attente = 0 ;
		scheduler();
		if(ADDR_VALIDE(retvalp))
			*retvalp = 
				table_process[current_process->buff_pid]
				->buff_val ;
		else if (retvalp!=NULL)
			ret_valide=0;	
		table_process[current_process->buff_pid]->etat = MORT ;
		lifo_add(&list_pid_dispo,current_process->buff_pid);
		queue_del(table_process[current_process->buff_pid],fils_suiv);
		return ret_valide==1?current_process->buff_pid:-1 ;
	} else {
		queue_for_each(fils, &current_process->list_fils, 
			       process, fils_suiv){
			if(fils->etat!=MORT && fils->pid == pid)
			{
				//traitement
				if(fils->etat != ZOMBIE){
					current_process->etat = BLOQUE_FILS ;
					current_process->attente = pid ;
					scheduler();
				}
				fils->etat = MORT ;
				lifo_add(&list_pid_dispo,fils->pid);
				if(ADDR_VALIDE(retvalp))
					*retvalp = fils->buff_val ;
				else if (retvalp!=NULL)
					ret_valide=0;
				queue_del(fils,fils_suiv);
				return ret_valide==1?pid:-1 ;
			}
		}
		return -1 ;
	}
}

process * getCurrentProcess()
{
	return table_process[pid_current_process] ;
}

void ps(void)
{
	printf(" PID  PRIO  COMMAND             STATES\n");
	for(int i = 1; i<NBPROC; i++)
	{
		if(table_process[i] != NULL && table_process[i]->etat != MORT && table_process[i]->etat != ZOMBIE)
		{
			printf("  %2i   %3i  %-20s", table_process[i]->pid, table_process[i]->prio,
					table_process[i]->name); 
			if(i == pid_current_process)
			{
				printf("Active\n");
			} else {
				switch(table_process[i]->etat){
				case 0 : printf("Waiting\n"); break;
				case 1 : printf("Blocked I/O\n"); break;
				case 2 : printf("Blocked on semaphore\n"); break;
				case 3 : printf("Waiting son(s)\n"); break;
				case 4 : printf("Sleeping\n"); break;
				default : break;
				}
			}
		}
	}
}

void sinfo(void)
{
	printf("------------------------------\n");
	printf("----- SEMAPHORES RUNNING -----\n");
	printf("------------------------------\n");
	for(int i = 0; i<NBSEMS; i++)
	{
		if(table_sem[i] != NULL)
		{
			printf("SEM ID         COUNT\n");
			printf(" %5i            %2i\n", i, table_sem[i]->count);
			if(!queue_empty(&table_sem[i]->process_bloque_sem))
			{
				printf("------------------------------\n");
				printf(" PROCESS BLOCKED\n");	
				printf("------------------------------\n");
				process * cour ;
				queue_for_each(cour,&table_sem[i]->process_bloque_sem,process,suiv)
				{
					printf(" PID  PRIO  COMMAND\n");
					printf("  %2i   %3i  %-20s\n", cour->pid, cour->prio,
					cour->name); 
				}
				printf("------------------------------\n");
			}
		}
		
	}
}

void pinfo(void)
{	
	process * cour ;
	printf("------------------------------\n");
	printf("----- MESSAGE FILES USED -----\n");
	printf("------------------------------\n");
	for(int i = 0; i<NBQUEUE; i++)
	{
		if(table_file[i] != NULL)
		{
			printf("FILE ID  TMP  NBMSG\n");
			printf("  %5i   %2i     %2i\n", i, table_file[i]->tampon_length, table_file[i]->nb_message);
			if(!queue_empty(&table_file[i]->receive))
			{
				printf("------------------------------\n"
				       " PROCESS BLOCKED ON RECEIVE\n"	
				       "------------------------------\n");
				queue_for_each(cour,&table_file[i]->receive,process,suiv)
				{
					printf(" PID  PRIO  COMMAND\n");
					printf("  %2i   %3i  %-20s\n", cour->pid, cour->prio,
					cour->name); 
				}
				printf("------------------------------\n");
			}
			if(!queue_empty(&table_file[i]->send))
			{
				printf("------------------------------\n");
				printf(" PROCESS BLOCKED ON SEND\n");	
				printf("------------------------------\n");
				queue_for_each(cour,&table_file[i]->send,process,suiv)
				{
					printf(" PID  PRIO  COMMAND\n");
					printf("  %2i   %3i  %-20s\n", cour->pid, cour->prio,
					cour->name); 
				}
				printf("------------------------------\n");
			}
		}
		
	}
}

void *syscall_sbrk(int increment)
{
	process * current_process = table_process[pid_current_process];
	char *s = current_process->current ;
	char *c = s + increment ;	
	if (c < current_process->current) {
                return ((void*)(-1));
        }
	while (c > current_process->heap_end){
		int * tmp ;
		if (palloc(&tmp)==-1)
			return ((void*)(-1));
		if (pmap((int***)current_process->PAGED,
		 tmp,
		 (int *)(current_process->heap_end),7)==-1)
			return ((void*)(-1));
		current_process->heap_end += KB4 ;
	}
        /* The heap grown */
	current_process->current = c;
	return s;
}
