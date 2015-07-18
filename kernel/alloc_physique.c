#include "alloc_physique.h"
#include <stddef.h>
#include <stdio.h>
#include "lifo.h"

#define PDENTRY(x) (((int)x & 0xffc00000)>>22)
#define PTENTRY(x) (((int)x & 0x003ff000)>>12)

link head;

void init_alloc_physique(void)
{
	lifo_init(&head);
	for (int i= MB256-KB4; i>= MB64; i=i-KB4){
		lifo_add(&head,i);
	}
}

int palloc(int **page)
{
	if (lifo_empty(&head))
		return -1;
	*page=(int *)lifo_out(&head);
	for (int i=0; i<KB4/4;i++)
		(*page)[i]=0;
	return 0;
}

int pfree(int *page)
{
	if ((int)page>MB256 || (int)page<MB64 || (int)page%KB4!=0)
		return -1;
	lifo_add(&head,(int)page);
	return 0;
}

int pmap(int ***paged,int * adrp, int * adrv, int p)
{
	if (paged==NULL)
		return -1;
	if ( paged[PDENTRY(adrv)]==NULL ){
		palloc((void *)(&(paged[PDENTRY(adrv)])));
	}
	paged[PDENTRY(adrv)]=
			(int**)(((int)paged[PDENTRY(adrv)] & 0xfffff000) | 7);
	*((int **)(((int)(paged[PDENTRY(adrv)]) & 0xfffff000) + 
		   sizeof(int)*PTENTRY(adrv)
		  ))=(int *)(((int)adrp & 0xfffff000) | (0xfff & p));
	return 0;
}

int *tradvp(int ***paged, int *adrv)
{
	if (paged==NULL || paged[PDENTRY(adrv)]==NULL)
		return NULL;
	return (int*)
		(( (int)(*((int **)(((int)(paged[PDENTRY(adrv)]) & 0xfffff000)
				    +sizeof(int)*PTENTRY(adrv)))) & 0xfffff000)
		 | (0xfff & (int)adrv));
}
