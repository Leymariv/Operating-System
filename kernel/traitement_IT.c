#include <stdlib.h>
#include <stdio.h>
#include "ecran.h"
#include "segment.h"
#include "cpu.h"
#include "process.h"
#include "processor_structs.h"

#define QUARTZ  0x1234DD
#define CLOCKFREQ 50
#define SCHEDFREQ 1

#define ADDR_VALIDE(addr) (unsigned)addr>=0x40000000

//compteur
static unsigned long compteur = 0 ;

/*
* Compte le nombre de secondes
**/
void affiche_temps(void)
{
	//acquitement
	outb(0x20, 0x20);

	compteur = compteur + 1 ;
	//chaine qui contiendra l heure
	char temps[32];
	
	if(compteur % CLOCKFREQ == 0)
	{
		//formater l affichage
		sprintf(temps, "uptime %02u : %02u : %02u", 
			(unsigned)((compteur / CLOCKFREQ)/3600), 
			(unsigned)((compteur / CLOCKFREQ)/60)%60,
			(unsigned)((compteur / CLOCKFREQ)%60));
		//fonction de placage a lecran
		afficher_heure(temps);	
	}
	//appel a l'ordonnanceur	
	if (compteur % SCHEDFREQ ==0)
		scheduler();
}

void init_traitant_IT(int num_IT, void (*traitant)(void), int mode)
{
	//on se place dans la table des interruptions
	unsigned *vect = (unsigned int *)(idt+num_IT);
	
	*vect = (unsigned)((KERNEL_CS << 16) | ((unsigned)traitant & 0xFFFF)) ;
	vect ++ ;
	*vect = (unsigned)(((unsigned)traitant & 0xFFFF0000) | mode);
}

void masque_IRQ(int num_IRQ, int masque)
{
	char mask = 0x01 ;
	unsigned char val = inb(0x21);
	mask = mask << num_IRQ ;
	if(masque){
		outb(val| mask, 0x21);
	}
	else
	{	
		mask = ~mask;
		outb(val & mask, 0x21);
	}
}

void clock_settings(unsigned long *quartz, unsigned long *ticks)
{
	if (ADDR_VALIDE(quartz) && ADDR_VALIDE(ticks)){
		*quartz = (unsigned long)QUARTZ;
		*ticks = (unsigned long)QUARTZ / (unsigned long)CLOCKFREQ;
	}
}

void init_clock()
{
	//on indique que l on va envoyer la valeur de reglage
	outb(0x34, 0x43);
	//8 bits de poids faibles de la valeur de reglage 
	outb((QUARTZ / CLOCKFREQ) % 256, 0x40);
	//8 bits de poids forts de la valeur de reglage
	outb((QUARTZ / CLOCKFREQ)/256, 0x40);
}

unsigned long current_clock()
{
	return compteur ;
}
