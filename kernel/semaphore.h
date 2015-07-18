#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#include "queue.h"

#define NBSEMS 10001

typedef struct sem_t {
	short int count ;
	link process_bloque_sem ;
	link process_del ;
	link process_reset ;
} sem_t ;

sem_t * table_sem[NBSEMS] ;


void init_semaphores(void);

/*
 * Retourne -1 si sem invalide
 * sinon 16 bits de poids fort à 0 et les 16 de points faible
 * sont la valeur du semaphore
 */
int scount(int sem);

/*
 * Alloue une structure de semaphore libre et l'initialise à 
 * la valeur count
 */
int screate(short int count);

/*
 * Fait passer tous les processus bloqué par sem à l'état Activable
 * 
 * Return -1 si sem invalide 
 * sinon 0
 */
int sdelete(int sem);

/*
 * Fait passer tous les processus bloqué par sem à l'état ACTIVABLE
 * et associe la valeur de count à sem
 *
 * Retourne -1 si count < 0 ou si sem invalide
 * sinon 0
 */
int sreset(int sem,short int count);

/*
 * Incremente de 1 la valeur du semaphore sem
 * 
 * Si count <= 0 après incrémentation, le premier processus bloqué dans la file
 * de sem passe dans l'état ACTIVABLE 
 */
int signal(int sem);

/*
 * Equivalent à count signal
 * 
 * Retourne -1 si sem invalide, -2 en cas de dépacement de capacité et 0 sinon
 */
int signaln(int sem, short int count);

/*
 * Test count du semaphore sem
 * Si count > 0, count --
 * Si <= 0, erreur retournee
 *
 * Retourne -1 si sem invalide, -3 si count <= 0, -1 en cas de dépacement de capacité
 * sinon 0
 */
int try_wait(int sem);

/*
 * Décrémente de 1 la valeur count du semaphore sem (count --). 
 *
 * Si dépassement de capacité, alors elle n'est pas effectuée
 * Sinon, 
 * 	si cout < 0, le processus passe de ACTIF à BLOQUE sur le 
 *         semaphore sem
 *
 * Retourne -1 si sem invalide, -2 en cas de dépassement de capacité,
 * -3 si réveil consécutif à sdelete, -4 s'il est consécutif à sreset
 * sinon 0
 */
int wait(int sem);

#endif
