#ifndef __READCMD_H__
#define __READCMD_H__

#define NBPARAMS 40 //nombre de parametres max d'une commande (théorique)

struct cmdline {
	char ** cmd ; /* une commande est un tableau de chaine de caractères
		       * le premier élément est la commande
		       * les suivants sont les paramètres 
		       */
	int nb_params ; //nombre de paramètres de la commande
} ;

struct cmdline *readcmd(char *prompt);

void free_cmd(struct cmdline *c);

#endif
