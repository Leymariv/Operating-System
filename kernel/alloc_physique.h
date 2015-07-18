#ifndef __ALLOC_PHYSIQUE_
#define __ALLOC_PHYSIQUE_

#define MB64 0x4000000
#define MB256 0x10000000
#define KB4 0x1000

/* Initialise l'allocateur de memoire physique utilisateur */
void init_alloc_physique(void);

/* Stock l'adresse d'une page disponible dans page, puis retourne 0
 * Si aucune page disponible, retourne -1
 */
int palloc(int **page);

/* Recupere une page dans la liste des pages dispo
 * Si la page ne fait pas partie de la bonne plage d'adresse retourne -1
 * (ou si elle n'est pas aligné sur 4Ko)
 * Sinon retourne 0
 */
int pfree(int *page);


/*
 * Map la l'adresse_physique "adrp" à l'adresse virtuelle "adrv" dans 
 * la page directory "paged" avec les permission "p"
 */
int pmap(int ***paged,int * adrp, int * adrv, int p);

/*
 * Permet de recuperer l'adresse physique à partir d'une adresse virtuelle
 * return NULL si l'adresse virtuelle n'est pas mappé
 */
int *tradvp(int ***paged, int *adrv);

#endif
