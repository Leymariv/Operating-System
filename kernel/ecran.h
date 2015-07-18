#ifndef __ECRAN_H__
#define __ECRAN_H__

//couleurs utiles
#define NOIR 0
#define ROUGE 4
#define BLANC 15
#define BLEU 9
#define VERT 10

#define BASE_ECRAN 0xB8000
#define LARGEUR_ECRAN 80
#define HAUTEUR_ECRAN 25

/* position courante du curseur */
extern int lig_;
extern int col_;

/*Necessaire pour l'appel systeme printf du mode user*/
void console_putbytes(char *chaine, int taille);

/*affiche l heure en bas a droite*/
void afficher_heure(char *chaine);

/*permet de changer la couleur d'affichage du texte du printf*/
void change_color(int color);

/*permet de deplacer de le curseur*/
void place_curseur(int lig, int col);

#endif
