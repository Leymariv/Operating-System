#include "lifo.h"
#include "mem.h"
#include <stddef.h>
#include <stdio.h>

typedef struct elem_{
	int val;
	link suiv;
} elem;

void lifo_init(link *head){
	INIT_LIST_HEAD(head);
}

int lifo_empty(link *head){
	return queue_empty(head);
}

void lifo_add(link *head, int valeur){
	elem *tmp=mem_alloc(sizeof(elem));
	tmp->val=valeur;
	queue_add(tmp,head,elem,suiv,val);
}

int lifo_out(link *head){
	if (queue_empty(head))
		return -1;
	elem *mem=queue_bottom(head,elem,suiv);
	int val=mem->val;
	queue_del(mem,suiv);
	mem_free(mem,sizeof(elem));
	return val;
}
