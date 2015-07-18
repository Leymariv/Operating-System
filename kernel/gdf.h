#ifndef __GDF_H_
#define __GDF_H_
#include "parseur_gdf.h"

/*
  Une liste de inodes disponible dans le kernel.
  Initialement la liste ne contient qu'une élément le seul inodes dispo.
  Lorsque quelqu'un demande un inode, si après retrait, il n'y a plus d'inodes on en 
  rajoute un de maniere itteligente. Lorsqu'un element est liberer, son inodes revient
  dans la liste.

  On met une table de hachage qui à partir d'un inodes (de dossier) nous permet 
  d'acceder la liste des elements compris dans ce dossier.

  Format d'un éléments :
     type (dossier ou fichier)
     numéro (inodes ou adresse du fichier)
     un tableau de char afin de stocké le nom de l'élément

  Format d'un fichier : Tout l'espace est destiné à stocké les données
  
 */

elem *dcour; /* Dossier courant du shell */

/* Initialise le gestionnaire de fichier */
void init_gdf();

/* Permet d'afficher les éléments en dessous du dossier courant 
 * retourne -1 si le dossier n'existe pas, 0 sinon
 */
int ls_gdf(char *name); 

/* Permet de recuperer le nom du dossier courant dans lequel nous sommes 
 * Le tampon fourni doit faire minimum 20 caratères
 */
void get_dcour(char *tampon);

/* Permet de deplacer le dossier courant 
 * Retourne -1 si le dossier dans lequel on veut aller n'existe pas, 0 sinon
 */
int cd_gdf(char *dossier);

/* Creer un dossier ou un fichier dans le repertoire courant 
 * Retourne -1 si le nom est deja utilisé ou est incorrect
 * 0 sinon
 */
int mkdir_gdf(char *name, char type_);

/* Supprime un dossier, ou fichier, dans le repertoire courant
 * retourne -1 si il n'existe pas, -2 si il est non vide, 0 sinon
 */
int rmdir_gdf(char *name, char type_);

/* sauvegarde l'affichage du shell, et lance l'editeur en changant l'echo 
 * et le cons_read.
 * retourne -1 si le fichier a editer n'existe pas
 * 0 sinon
 */
int editor_on(char *name);

/* remet l'affichage du shell, et remet l'echo et le cons_read initial */
void editor_off();

/* Affiche le contenu d'un fichier 
 * renvoi -1 si le fichier n'existe pas
 * 0 sinon
 */
int cat_gdf(char *name);

/*retourne un descripteur sur un fichier, ou NULL en cas d'erreur */
FILE *fopen_gdf(const char *path, const char *mode);

/*free les structure du descripteur */
int fclose_gdf(FILE *fp);

/* The  function  fread()  reads nmemb elements of data, each size bytes long, from the stream pointed to by stream, storing them at the
   location given by ptr.*/
int fread_gdf(void *ptr, int size, int nmemb, FILE *stream);

/* The function fwrite() writes nmemb elements of data, each size bytes long, to the stream pointed to by stream,  obtaining  them  from
   the location given by ptr. */
int fwrite_gdf(const void *ptr, int size, int nmemb, FILE *stream);

#endif
