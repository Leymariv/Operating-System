#ifndef __KEYBOARD_H_
#define __KEYBOARD_H_

/*echo standard */
void echo_fct_standard(char * str, int fid, int echo);

/* Permet de changer le formatage de l'echo */
void (*echo_fct)(char *, int, int);

/*
 * Initialise le tampon dans lequel les caractères seront stockés lors
 * d'interruptions
 */
void init_keyboard();

/*
 * Acquitte la reception d'interruption, et appel "do_scancode" avec les
 * bon paramètres
 */
void scan_keyboard();

/*
 * Modifie la variable global d'echo
 */
void cons_echo(int on);

/*
 * attend une chaine d'un maximum length caractères, ou bien d'une chaine 
 * finissant par un "ENTREE"
 * retourne la taille exacte de la chaine recupere
 */
unsigned long cons_read(char *string, unsigned long length);

/*
 * Affiche caractères par caractères, la chaine demandée
 */
int cons_write(const char *str, long size);

/*
 * recupere les caractère et les stocks dans le tampon
 * si le mode echo est activée, il affiche en plus les caractères selon
 * les modalités du cahier des charges
 */
void keyboard_data(char *str);

/*
 * envoi 0xed sur le port 0x60 pour prevenir le changement d'état des leds
 * puis envoi leds sur 0x60 pour les mettre à jour
 */
void kbd_leds(unsigned char leds);

#endif
