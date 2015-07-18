#ifndef __LIFO__
#define __LIFO__
#include <queue.h>

/*
 * Paquetages permettant de crées une listes de nombres.
 * ces nombres sont classés par ordre croissant.
 * l'insertion, est une insertion trier.
 * le retrait se fait avec l'element de tete.
 */


/*
 * Initialise la tete de la liste
 */
void lifo_init(link *head);

/*
 * retourne 1 si la liste est vide
 * 0 sinon
 */
int lifo_empty(link *head);

/*
 * ajout un element de valeur "valeur" a la liste
 */
void lifo_add(link *head, int valeur);

/*
 * retire l'element de tete et retourne sa valeur
 * si la liste est vide, retourne -1
 */
int lifo_out(link *head);	

#endif
