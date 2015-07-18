#include "debugger.h"
#include "cpu.h"
#include <stdio.h>
#include "traitement_IT.h"
#include "process.h"
#include "alloc_physique.h"
#include "message_file.h"
#include "semaphore.h"
#include "shm.h"
#include <stddef.h>
#include "keyboard.h"
#include "gdf.h"

extern void traitant_horloge();
extern void traitant_syscall();
extern void traitant_keyboard();

static void traitant_13_14()
{
	printf("Segmentation fault\n");
	kill(getpid());
}

static void division_zero()
{
	printf("Division par zero\n");
	kill(getpid());
}

static void bound_range()
{
	printf("Bound range exceeded\n");
	kill(getpid());
}

static void overflow()
{
	printf("Overflow\n");
	kill(getpid());
}

void idle(void)
{
	while(1) {
		sti();
		hlt();
		cli();
	}
}

void kernel_start(void)
{
	printf("\fInitialisation de l'horloge... ");
	init_clock();
	printf("OK\nDemasquage des interruptions horloge et clavier... ");
	masque_IRQ(32,0);
	masque_IRQ(1,0);
	masque_IRQ(12,0);
	printf("OK\nInitialisation des programmes d'interruptions... ");
	init_traitant_IT(32, traitant_horloge,0x8E00);
	init_traitant_IT(49, traitant_syscall,0xEE00);
	init_traitant_IT(33, traitant_keyboard,0x8E00);
	init_traitant_IT(13, traitant_13_14,0x8E00);
	init_traitant_IT(14, traitant_13_14,0x8E00);
	init_traitant_IT(0, division_zero,0x8E00);
	init_traitant_IT(4, overflow,0x8E00);
	init_traitant_IT(5, bound_range,0x8E00);
	printf("OK\nInitialisation de la liste des applications... ");
	init_table_symbols();
	printf("OK\nInitialisation de l'allocateur de memoire... ");
	init_alloc_physique();
	printf("OK\nInitialisation de l'ordonnanceur... ");
	init_scheduler();
	printf("OK\nInitialisation de la liste des pages partagees... ");
	init_shm();
	printf("OK\nInitialisation du gestionnaire de file de message... ");
	init_message_file();
	printf("OK\nInitialisation du gestionnaire de semaphores... ");
	init_semaphores();
	printf("OK\nInitialisation du clavier... ");
	init_keyboard();
	printf("OK\nInitialisation du gestionnaire de fichiers... ");
	init_gdf();
	printf("OK\f");
	start("help",3000,200,NULL);
	start("shell", 15000, 128, NULL);
	idle();
}
