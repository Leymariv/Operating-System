#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <queue.h>

#define NB_REGS 7
#define NBPROC 30
#define MAXPRIO 255
#define ESP regs[3]
#define PAGED regs[5]
#define ESP0 regs[6]

int *eip_dep;

enum Etat_process {
	ACTIVABLE,
	BLOQUE_IO,
	BLOQUE_SEM,
	BLOQUE_FILS,
	ENDORMI,
	ZOMBIE,
	MORT
};

/*structure d'un processus leger*/
typedef struct process_ {
	int pid ; 
	int regs[NB_REGS] ; //zone de sauvegarde des registres
	enum Etat_process etat ; //etat du processus
	unsigned long hreveil ; //heure de reveil du processus
	int prio ; //priorite du processus
	const char* name;
	int buff_pid; // buffer contenant le pid du fils qui renvoit la valeur
	int buff_val; // buffer contenant la valeur renvoyée au père
	link suiv;
	/* pour gerer les files de messages et semaphores */
	int id_fs; /* numéro de la file ou du semaphore dans laquel/lequel le processus est bloqué */
	link reset_suiv; /* permet de chainer les processus bloqué lors d'un reset */
	link del_suiv; /* permet de chainer les processus bloqué lors d'un delete */
	/* pour gerer la filiation */
	struct process_ * pere ;
	link list_fils ;
	link fils_suiv ;
	/* pour le waitpid */
	int attente ; // -1 personne, 0 n'importe lequel et pid à attendre sinon
	/* gestion du tas de l'application*/
	char* current ;
	char* heap_end ;
} process ;

/*
 * fonction qui initialise une table de haschage avec tout les application pouvant
 * etre lancer
 */
void init_table_symbols(void);

/*
* Creation d'un processus de code pt_func avec allocation d'une zone memoire
* de taille ssize, de priorité prio, de nom name et d'argument arg
*
* Retourne le PID du processus
*/
int start(const char *name, unsigned long ssize, int prio, void *arg);

/*
* Donne la priorité newprio au processus identifié par la valeur de pid. 
* Si la priorité du processus change et qu'il était en attente dans une file, 
* il doit y être replacé selon sa nouvelle priorité.
*
* Si la valeur de newprio ou de pid est invalide, 
* la valeur de retour de chprio est strictement négative, 
* sinon elle est égale à l'ancienne priorité du processus pid. 
*/
int chprio(int pid, int newprio);

/* 
* Si la valeur de pid est invalide, la valeur de retour est strictement négative 
* sinon elle est égale à la priorité du processus identifié par la valeur pid.
*/
int getprio(int pid);

/*
* Passe le processus dans l'état endormi jusqu'à ce que un temps de clock 
* soit passé. 
*/
void wait_clock(unsigned long clock);

/*retourne le pid du processus courant*/
int getpid(void);

/*fonction permetttant de schedulé un processus particulier 
 *(necessaire pour les files de messages) */
void scheduler_process(int pid);

/*fonction d'ordonnancement*/
void scheduler(void);

/*initialise l'ordonnancement*/
void init_scheduler(void);

/* 
* kill le process de pid pid 
* return 0 en cas de succes et -1 si le pid
* passé est invalide
*/
int kill(int pid);

/*
 * fait passer le processus dans la liste des process bloqué en IO (ne reschedule pas)
 */
process *bloque_process(int num);

/*
 * remet le processus dans la liste des processus activable (ne reschedule pas)
 */
void debloque_process(int pid);

/*
 * Bloque le processus courant sur le semaphore sem
 */
process *bloque_process_sem(int sem);

/*
* Le processus courant est terminé normalement 
* la valeur retval est passée à son père quand il appelle waitpid
*/
void exit(int retval);

/*
* Si pid négatif, le processus appelant attend qu'un de ses fils, 
* n'importe lequel, soit terminé et récupère sa valeur de retour dans *retvalp,
* à moins que retvalp soit nul.
*
* Si pid positif, le processus attend que son fils de pid pid soit terminé
* ou tué et récupère sa valeur de retour dans *retvalp
* 
* si aucun fils n'existe ou si le pid passé n'est pas celui d'un fils
* retourne un entier négatif 
* sinon retourne le pid du fils dont la valeur de retour a été récupérée
*/
int waitpid(int pid, int *retvalp);

/*
 * Retourne un pointeur vers le process courant
 */
process * getCurrentProcess();

/* 
 * Appel système permettant d'afficher les processus
 * en cours d'exécution
 */
void ps(void);

/* 
 * Appel système permettant d'afficher les semaphores
 * utilisées
 */
void sinfo(void);

/* 
 * Appel système permettant d'afficher les files de 
 * messages utilisées
 */
void pinfo(void);

/*	
 * Appel système permettant d'allouer dynamiquement
 * de la mémoire en mode USER 
 *
 * Agrandit le tas de l'application et alloue de 
 * nouvelles pages si necessaire
 *
 * Retourne un pointeur sur le début du tas
 */
void *syscall_sbrk(int increment);

#endif
