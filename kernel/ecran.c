#include "ecran.h"
#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include "process.h"
#include "shm.h"

//ports 
#define CMD_PORT (unsigned short)0x3D4
#define DATA_PORT (unsigned short)0x3D5

//largeur tabulation
#define LARGEUR_TAB 8

int lig_=0;
int col_=0;

int print_addr=BASE_ECRAN;

/* Couleur courante d'ecriture */
static int _color=BLANC;

short *ptr_mem(int lig, int col)
{
	return (short*)(print_addr + 2 * (lig * LARGEUR_ECRAN + col));
}

void ecrit_car(int lig, int col, char c, int ctext, int cfond, int cl)
{
	*ptr_mem(lig, col) = c|(ctext<<8)|(cfond<<12)|(cl<<15);
}

void defilement(void)
{
	memmove((void*)print_addr,
		(void*)(print_addr + 2 *LARGEUR_ECRAN), 
		LARGEUR_ECRAN * (HAUTEUR_ECRAN - 2) * 2);
	for(int i = 0; i < LARGEUR_ECRAN; i++)
	{
		*(ptr_mem(HAUTEUR_ECRAN - 2, i)) = 0xF00 ;
	}
}

void place_curseur(int lig, int col)
{
	short pos ;
	//calcul de la position du curseur en memoire
	pos = col + lig * LARGEUR_ECRAN ;
	
	//on indique que l on va envoyer la partie basse de la postion du curseur
	outb(0x0F, CMD_PORT);
	outb((unsigned char)(pos & 0xFF), DATA_PORT);
	
	//on indique que l on va envoyer la partie haute de la postion du curseur
	outb(0x0E, CMD_PORT);
	outb((unsigned char)(pos >> 8), DATA_PORT);

	lig_ = lig ;
	col_ = col ;
}

void efface_ecran(void)
{
	for(int i = 0; i < LARGEUR_ECRAN *  HAUTEUR_ECRAN; i++)
	{
		*(ptr_mem(0, i)) = 0xF00 ;
	}
	lig_ = 0 ;
	col_ = 0 ;
}

void traite_car(char c)
{
	if((int)c < 127)
	{
		int tab_suivante;
		switch((int)c)
		{
			case 8 : 
				if( col_ != 0)
				{
					col_ = col_ - 1 ;
					place_curseur(lig_, col_);
				}
			
				break ;
			case 9 :
				tab_suivante = (int) (col_ / LARGEUR_TAB) + 1 ;
				col_=tab_suivante*LARGEUR_TAB;
				if (col_ >= 80){
					col_=0;
					lig_++;
					if (lig_>=HAUTEUR_ECRAN-1){
						lig_--;
						defilement();
					}
				}
				place_curseur(lig_, col_);
				break ;
			case 10 :
				col_ = 0 ;
				lig_ = lig_ + 1 ;
				if(lig_ >= HAUTEUR_ECRAN-1)
				{
					lig_--;
					defilement();
				}
				place_curseur(lig_, col_);
				break ;
			case 12 : 
				efface_ecran();
				place_curseur(lig_, col_);
				break ;
			case 13 : 
				col_ = 0 ;
				place_curseur(lig_, col_);
				break ;
			default :
				ecrit_car(lig_, col_, c, _color, NOIR, 0);
				col_ ++;
				if(col_ >= 80)
				{
					col_ = 0;
					lig_ ++;
					if(lig_ >= HAUTEUR_ECRAN - 1)
					{
						lig_ -- ;
						defilement();
					}
				}
				place_curseur(lig_, col_);
		}
	}
}

void console_putbytes(char *chaine, int taille)
{
	for(int i = 0; i < taille; i++)
	{
		traite_car(chaine[i]);
	}
}

void afficher_heure(char *chaine)
{
	int length = strlen(chaine);
	int ancre = LARGEUR_ECRAN - length - 1 ;
	for(int i = 0; i < length; i++)
	{
		ecrit_car(HAUTEUR_ECRAN-1, 
			  ancre + i, 
			  chaine[i], 
			  ROUGE, 
			  NOIR, 
			  0);
	}
}

void change_color(int color)
{
	_color=color;
}
