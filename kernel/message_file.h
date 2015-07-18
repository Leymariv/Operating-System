#ifndef __MESSAGE_FILE_H__
#define __MESSAGE_FILE_H__
#include "queue.h"

#define NBQUEUE 10

typedef struct file_{
	int *tampon;       /*tampon de la file*/
	int tampon_length; /*taille du tampon*/
	int nb_message;    /*nombre de message present dans la file*/
	int cour;          /*indice courant de la file pour l'insertion*/
	link receive;      /* liste des process bloqué en attente de message*/
	link send;         /* liste des process bloqué en attente pour deposer un message*/
	link reset;        /* liste des process present lors du reset n'ayant pas encore redemarrer*/
	link del;          /* liste des process bloqué dans la file au moment du delete*/
} file;

/*table des files*/
file * table_file[NBQUEUE];

/* initialise une liste d'identifiants (NBQUEUE) disponibles */
void init_message_file(void);

/*
 * Alloue une file de message de capacité count
 * 
 * Retourne -1 s'il n'y a plus de file disponible ou si count <= 0
 * sinon retourne l'identifiant de file allouée
 */
int pcreate(int count);

/*
 * Lit la quantité de données et de processus en attente sur la file fid.
 *
 * Si count != 0, count reçoit une valeur négative égale à l'opposé du nombre 
 * de processus bloqués sur file vide ou une valeur positive égale à la somme du nombre 
 * de messages dans la file et du nombre de processus bloqués sur file pleine.
 *
 * Retourne -1 si fid invalide
 * sinon 0
 */ 
int pcount(int fid, int *count);

/*
 * Détruit la file de messages identifiée par fid 
 * et fait passer dans l'état activable tous les processus 
 * qui se trouvaient bloqués sur la file
 *
 * Retourne -1 si fid invalide
 * sinon 0
 */
int pdelete(int fid);

/*
 * Lit le premier message de la file fid.
 * Le message lu est placé dans *message si message n'est pas nul, sinon il est oublié.
 * Keyboard permet de savoir, si le preceive est fait par le clavier, ou par une
 * application utiliseur
 *
 * Retourne -1 si fid invalide
 * sinon 0
 */
int preceive(int fid, int *message,int keyboard);

/* 
 * Vide la file identifiée par la valeur de fid et fait passer dans l'état activable tous les processus 
 * se trouvant dans l'état bloqué sur file pleine ou dans l'état bloqué sur file vide.
 *
 * Retourne -1 si fid invalide
 * sinon 0
 */
int preset(int fid);

/* 
 * Envoie le message dans la file identifiée par fid
 *
 * Retourne -1 si fid invalide
 * sinon 0
 */
int psend(int fid, int message);

#endif
