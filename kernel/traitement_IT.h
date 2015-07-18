#ifndef __TRAITEMENT_IT_H__
#define __TRAITEMENT_IT_H__

/* affiche le temps toutes les secondes */
void affiche_temps(void);

/*initialise le traitant dinterruption dans la table correspondante*/
void init_traitant_IT(int num_IT, void (*traitant)(void),int mode);

/* masque ou demasque une interuption */
void masque_IRQ(int num_IRQ, int masque);

/*initialise l horloge*/
void init_clock();
void clock_settings(unsigned long *quartz, unsigned long *ticks);

/* retourne le temps courant */
unsigned long current_clock();

#endif
