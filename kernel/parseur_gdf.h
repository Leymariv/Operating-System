#ifndef _GESTIONNAIRE_FICHIERS_
#define _GESTIONNAIRE_FICHIERS_

#include <queue.h>

#define TAILLE_MAX_NAME 20

typedef struct elem_ {
	int type; /* Permet de savoir si l'on considere un dossier ou un fichier 
		   * d pour un dossier
		   * f pour un fichier
		   */
	void * id; /*l'adresse réel pour un fichier */
	int last_line; /*permet de savoir la dernier ligne interessante d'un fichier */
	char name[TAILLE_MAX_NAME]; /* Nom de l'élément */
	struct elem_ *root; /*pointeur vers la racine du gestionnaire de fichier */
	struct elem_ * father; /* pointeur vers le pere */
	link sous_elem; /* Liste des éléments compris dans le dossier */
	link suiv; /* Permet de chainé les éléments compris dans le même dossier */
} elem;

/*Decsripteur de fichier */
typedef struct FILE_ {
	elem *pt_elem;
	void *indice;
	int type_file;
	int right;
} FILE;

int valide_name(char *name);

int end_word(char *name);

elem *file2path(const char *path, int mode, elem *dossier_courant, char * name);

#endif
