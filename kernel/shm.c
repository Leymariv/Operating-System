#include "hash.h"
#include "shm.h"
#include "alloc_physique.h"
#include "process.h"
#include "mem.h"
#include <stddef.h>

typedef struct _shm_t {
	void* addr ; /* adresse de début de la page */
	int count ;  /* nombre total de référence sur la page */
} shm_t ;

/* 
 * table de hashage contenant toutes les pages partagées
 * référencées par une chaine de caractère (key)
 */
hash_t *hash_shm ; 

void init_shm()
{
	hash_shm=mem_alloc(sizeof(hash_t));
	hash_init_string(hash_shm);
}

void *shm_create(const char *key)
{
	process * cour = getCurrentProcess();
	void *real_key=tradvp((int***)cour->PAGED,(void*)key);
	if(hash_isset(hash_shm, real_key) != 0)
		return NULL ;
	int* addr ;
	if(palloc(&addr) == -1)
		return NULL ;
	if(pmap((int ***)cour->PAGED, addr, addr, 7) == -1)
	{
		pfree(addr);
		return NULL ;
	}
	shm_t * sh = mem_alloc(sizeof(shm_t));
	sh-> addr = (void*)addr ;
	sh->count = 1 ; 
	if(hash_set(hash_shm, real_key, (void*)sh) != 0)
	{
		pfree(addr);
		mem_free(sh, sizeof(shm_t));
		return NULL ;
	}
	assert(hash_get(hash_shm, real_key, NULL)!=NULL);
	return sh-> addr ;
}

void *shm_acquire(const char *key)
{
	process * cour = getCurrentProcess();
	void *real_key=tradvp((int***)cour->PAGED,(void*)key);
	shm_t * sh ;
	if((sh=hash_get(hash_shm, real_key, NULL)) ==NULL )
		return NULL ;
	if(pmap((int ***)cour->PAGED, (int*)(sh->addr), (int*)(sh->addr), 7) == -1)
		return NULL ;
	++(sh->count);
	return sh->addr ;
}

void shm_release(const char *key)
{
	process * cour = getCurrentProcess();
	void *real_key=tradvp((int***)cour->PAGED,(void *)key);
	shm_t * sh ;
	if((sh =hash_get(hash_shm, real_key, NULL))==NULL)
		return ;
	//modification des droits sur la page pour le process courant
	if(pmap((int ***)cour->PAGED, (int*)(sh->addr), (int*)(sh->addr), 7) == -1)
	  return ;
	--(sh->count);
	if(sh->count == 0)
	{
		pfree(sh->addr);
		assert(hash_del(hash_shm, real_key) == 0);
		mem_free(sh, sizeof(shm_t));
	}
}
