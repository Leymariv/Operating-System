#include "message_file.h"
#include "process.h"
#include "mem.h"
#include "lifo.h"
#include <stddef.h>


#define SIZE_MAX_QUEUE 10000
#define ADDR_VALIDE(addr) (unsigned)addr>=0x40000000

link list_fid_dispo; /* liste des identifiants de file disponibles */

void init_message_file(void)
{
	lifo_init(&list_fid_dispo);
	for (int i=NBQUEUE-1; i>=0; i--)
		lifo_add(&list_fid_dispo,i);
}


int pcreate(int count)
{
	if (count < 1 || count > SIZE_MAX_QUEUE)
		return -1;
	if (lifo_empty(&list_fid_dispo))
		return -1;
	int i=lifo_out(&list_fid_dispo);
	table_file[i]=mem_alloc(sizeof(file));
	table_file[i]->tampon=mem_alloc(count*sizeof(int));
	table_file[i]->nb_message=0;
	table_file[i]->tampon_length=count;
	table_file[i]->cour=0;
	INIT_LIST_HEAD(&table_file[i]->reset);
	INIT_LIST_HEAD(&table_file[i]->receive);
	INIT_LIST_HEAD(&table_file[i]->send);
	INIT_LIST_HEAD(&table_file[i]->del);
	return i;
}

int pdelete(int fid)
{
	if (fid < 0 || fid > NBQUEUE -1 
		    || table_file[fid]==NULL 
                    || !queue_empty(&table_file[fid]->del))
		return -1;
	while (!queue_empty(&table_file[fid]->receive)){
		process * cour=queue_out(&table_file[fid]->receive,
					 process, 
					 suiv);
		debloque_process(cour->pid);
		queue_add(cour,
			  &table_file[fid]->del,
			  process,
			  del_suiv, 
			  prio);
	}
	while (!queue_empty(&table_file[fid]->send)){
		process * cour=queue_out(&table_file[fid]->send,
					 process,
					 suiv);
		debloque_process(cour->pid);
		queue_add(cour,
			  &table_file[fid]->del,
			  process,
			  del_suiv, 
			  prio);
	}
	while (!queue_empty(&table_file[fid]->reset)){
		process * cour=queue_out(&table_file[fid]->reset,
					 process,
					 suiv);
		debloque_process(cour->pid);
		queue_add(cour,&table_file[fid]->del,process,del_suiv, prio);
	}
	mem_free(table_file[fid]->tampon,
		 table_file[fid]->tampon_length*sizeof(int));
	table_file[fid]->nb_message=0;
	table_file[fid]->cour=0;
	if (queue_empty(&table_file[fid]->del)){
		mem_free(table_file[fid],sizeof(file));
		table_file[fid]=NULL;
		lifo_add(&list_fid_dispo,fid);
	}
	scheduler();
	return 0;
}

int preset(int fid)
{
	if (fid < 0 || 
	    fid > NBQUEUE -1 || 
	    table_file[fid]==NULL || 
	    !queue_empty(&table_file[fid]->del))
		return -1;
	table_file[fid]->nb_message=0;
	table_file[fid]->cour=0;
	while (!queue_empty(&table_file[fid]->receive)){
		process * cour=(process*)queue_out(&table_file[fid]->receive,
						   process,
						   suiv);
		debloque_process(cour->pid);
		queue_add(cour,
			  &table_file[fid]->reset,
			  process,
			  reset_suiv, 
			  prio);
	}
	while (!queue_empty(&table_file[fid]->send)){
		process * cour=(process*)queue_out(&table_file[fid]->send,
						   process,
						   suiv);
		debloque_process(cour->pid);
		queue_add(cour,
			  &table_file[fid]->reset,
			  process,
			  reset_suiv, 
			  prio);
	}
	scheduler();
	return 0;
}

int psend(int fid, int message)
{
	if (fid < 0 || 
	    fid > NBQUEUE -1 || 
	    table_file[fid]==NULL || 
	    !queue_empty(&table_file[fid]->del))
		return -1;
	while (table_file[fid]->nb_message==table_file[fid]->tampon_length){
		process *cour=bloque_process(-(fid+1));
		queue_add(cour,&table_file[fid]->send,process,suiv,prio);
		scheduler();
		if (!queue_empty(&table_file[fid]->del) && 
		    cour->del_suiv.next!=NULL){
			queue_del(cour,del_suiv);
			if (queue_empty(&table_file[fid]->del)){
				mem_free(table_file[fid],sizeof(file));
				table_file[fid]=NULL;
				lifo_add(&list_fid_dispo,fid);
			}
			return -1;
		} else if (!queue_empty(&table_file[fid]->reset) 
			   && cour->reset_suiv.next!=NULL){
			queue_del(cour,reset_suiv);
			return -1;
		}
	}
	table_file[fid]->tampon[table_file[fid]->cour]=message;
	table_file[fid]->cour=
		(table_file[fid]->cour+1)%table_file[fid]->tampon_length;
	table_file[fid]->nb_message++;
	if (!queue_empty(&table_file[fid]->receive)){
		process * p=(process*)queue_out(&table_file[fid]->receive,
						process,
						suiv);
		debloque_process(p->pid);
		scheduler_process(p->pid);
	}
	scheduler();
	return 0;
}

int preceive(int fid, int *message, int keyboard)
{
	if (fid < 0 || fid > NBQUEUE -1 || 
	    table_file[fid]==NULL || !queue_empty(&table_file[fid]->del))
		return -1;
	while (table_file[fid]->nb_message==0){
		process *cour=bloque_process(fid+1);
		queue_add(cour,&table_file[fid]->receive,process,suiv,prio);
		scheduler();
		if (!queue_empty(&table_file[fid]->del) && 
		    cour->del_suiv.next!=NULL){
			queue_del(cour,del_suiv);
			if (queue_empty(&table_file[fid]->del)){
				mem_free(table_file[fid],sizeof(file));
				table_file[fid]=NULL;
				lifo_add(&list_fid_dispo,fid);
			}
			return -1;
		} else if (!queue_empty(&table_file[fid]->reset) && 
			   cour->reset_suiv.next!=NULL){
			queue_del(cour,reset_suiv);
			return -1;
		}
	}
	if (ADDR_VALIDE(message) || (keyboard==1 && message!=NULL))
		*message=table_file[fid]->tampon[
			table_file[fid]->cour>=table_file[fid]->nb_message?
			table_file[fid]->cour-table_file[fid]->nb_message:
			table_file[fid]->cour-table_file[fid]->nb_message+
			table_file[fid]->tampon_length];
	table_file[fid]->nb_message--;
	if (!queue_empty(&table_file[fid]->send)){
		process * p=(process*)queue_out(&table_file[fid]->send,
						process,
						suiv);
		debloque_process(p->pid);
		scheduler_process(p->pid);
	}
	scheduler();
	return 0;
}

int pcount(int fid, int *count)
{
     	if (fid < 0 || fid > NBQUEUE -1 || 
	    table_file[fid]==NULL || !queue_empty(&table_file[fid]->del))
		return -1;
	if (ADDR_VALIDE(count)){
		int nb_proc_receive=0;
		int nb_proc_send=0;
		process *cour;
		queue_for_each(cour,&table_file[fid]->receive,process,suiv)
			nb_proc_receive++;
		queue_for_each(cour,&table_file[fid]->send,process,suiv)
			nb_proc_send++;
		*count=nb_proc_send+table_file[fid]->nb_message>0?
			nb_proc_send+table_file[fid]->nb_message:
			-nb_proc_receive;
	}
	return 0;
}
