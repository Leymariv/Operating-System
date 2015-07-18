#include "semaphore.h"
#include "process.h"
#include "lifo.h"
#include "mem.h"
#include <stddef.h>

#define SHRT_MAX_VALUE 32767 //((1 << (sizeof(short) << 3)) - 1)
#define SHRT_MIN_VALUE -32768

link list_sem_dispo;

void init_semaphores(void)
{
	lifo_init(&list_sem_dispo);
	for (int i=NBSEMS-1; i>=0; i--)
		lifo_add(&list_sem_dispo,i);
}

int screate(short int count)
{
	if(count < 0)
	{
		return -1 ;
	}
	if (lifo_empty(&list_sem_dispo))
		return -1;
	int i=lifo_out(&list_sem_dispo);
	table_sem[i] = mem_alloc(sizeof(sem_t));
	table_sem[i]->count = count ;
	INIT_LIST_HEAD(&table_sem[i]->process_bloque_sem);
	INIT_LIST_HEAD(&table_sem[i]->process_del);
	INIT_LIST_HEAD(&table_sem[i]->process_reset);
	return i ;
}

int scount(int sem)
{
	if(sem < 0 || sem > NBSEMS - 1 || table_sem[sem] == NULL)
		return -1 ;
	return (table_sem[sem]->count) & 0xFFFF;
}

int sdelete(int sem)
{	
	if(sem < 0 || sem > NBSEMS - 1 || table_sem[sem] == NULL || ! queue_empty(&table_sem[sem]->process_del))
		return -1 ;
	process *cour ;
	while(! queue_empty(&table_sem[sem]->process_bloque_sem))
	{
		cour=(process *)queue_out(&table_sem[sem]->process_bloque_sem,process, suiv);
		debloque_process(cour->pid);
		queue_add(cour, &table_sem[sem]->process_del, process, del_suiv, prio);  
	}
	while(! queue_empty(&table_sem[sem]->process_reset))
	{
		cour= queue_out(&table_sem[sem]->process_reset, process, reset_suiv);
		debloque_process(cour->pid);
		queue_add(cour, &table_sem[sem]->process_del, process, del_suiv, prio);
	}
	if (queue_empty(&table_sem[sem]->process_del)){
		lifo_add(&list_sem_dispo,sem);
		mem_free(table_sem[sem], sizeof(sem_t));
		table_sem[sem] = NULL;
	}
	scheduler();
	return 0 ;
}

int sreset(int sem, short int count)
{
	if(sem < 0 || sem > NBSEMS - 1 || table_sem[sem] == NULL || ! queue_empty(&table_sem[sem]->process_del))
		return -1 ;
	if(count < 0)
		return -1 ;
	process *cour ;
	table_sem[sem]->count = count ;
	while(! queue_empty(&table_sem[sem]->process_bloque_sem))
	{
		cour=(process *)queue_out(&table_sem[sem]->process_bloque_sem,process, suiv);
		debloque_process(cour->pid);
		queue_add(cour, &table_sem[sem]->process_reset, process, reset_suiv, prio);  
	}
	scheduler();
	return 0 ;
}

int signal(int sem)
{
	if(sem < 0 || sem > NBSEMS - 1 || table_sem[sem] == NULL || ! queue_empty(&table_sem[sem]->process_del))
		return -1 ;
	if(table_sem[sem]->count == SHRT_MAX_VALUE)
		return -2 ;
	table_sem[sem]->count += 1 ;
	if(table_sem[sem]->count <= 0)
	{
		process *cour = queue_out(&table_sem[sem]->process_bloque_sem, process, suiv);
		debloque_process(cour->pid);
		scheduler();
	}
	return 0 ;	
}

int signaln(int sem, short int count)
{
	int sched = 0 ;
	if(sem < 0 || sem > NBSEMS - 1 || table_sem[sem] == NULL || ! queue_empty(&table_sem[sem]->process_del) ||count < 0)
		return -1 ;
	if(table_sem[sem]->count + count > SHRT_MAX_VALUE)
		return -2 ;
	for(int i = 0; i < count; i++)
	{
		table_sem[sem]->count += 1 ;
		if(table_sem[sem]->count <= 0)
		{
			sched = 1 ;
			process *cour = queue_out(&table_sem[sem]->process_bloque_sem,process, suiv);
			debloque_process(cour->pid);
		}
	}
	if(sched == 1)
		scheduler();	
	return 0 ;	
}

int try_wait(int sem)
{
	if(sem < 0 || sem > NBSEMS - 1 || table_sem[sem] == NULL || !queue_empty(&table_sem[sem]->process_del))
		return -1 ;
	if(table_sem[sem]->count == SHRT_MIN_VALUE)
		return -2 ;	
	if(table_sem[sem]->count -1 < 0)
		return -3 ;	
	(table_sem[sem]->count --) ;
	return 0 ;	
}

int wait(int sem)
{	
	if(sem < 0 || sem > (NBSEMS - 1) || table_sem[sem] == NULL || !queue_empty(&table_sem[sem]->process_del))
	{
		return -1 ;
	}
	if(table_sem[sem]->count == SHRT_MIN_VALUE)
	{
		return -2 ;
	}
	table_sem[sem]->count -- ;
	if(table_sem[sem]->count < 0)
	{
		process *cour = bloque_process_sem(sem);
		queue_add(cour,&table_sem[sem]->process_bloque_sem,process,suiv,prio);
		scheduler();
		if (table_sem[sem]==NULL)
			return 0;		
		if (!queue_empty(&table_sem[sem]->process_del) && cour->del_suiv.next!=NULL){
			queue_del(cour,del_suiv);
			if (queue_empty(&table_sem[sem]->process_del)){
				lifo_add(&list_sem_dispo,sem);
				mem_free(table_sem[sem], sizeof(sem_t));
				table_sem[sem] = NULL;
			}
			return -3;
		} else if (!queue_empty(&table_sem[sem]->process_reset) && cour->reset_suiv.next!=NULL){
			queue_del(cour,reset_suiv);	
			return -4;
		}	 
	}

	return 0 ;
}
